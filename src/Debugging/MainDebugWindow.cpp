#include <QtWidgets>
#include <QString>
#include <QDebug>

#include <opencv2/core/core.hpp>

#include <iostream>

#include "MainDebugWindow.hpp"
#include "ui_MainDebugWindow.h"
#include "../Utilities/Frame.hpp"

MainDebugWindow::MainDebugWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDebugWindow)
{
    ui->setupUi(this);
    setWindowTitle("DenseDebugger");
    isRecordToFiles = false;
    videoWriter = 0;
}

MainDebugWindow::~MainDebugWindow()
{
    /*
    for(int c = 0; c < program->getFrames()->getCurrent().getCameras().size()*3; c++) {
        videoWriter[c].release();
    }
    delete [] videoWriter;
    */
    delete ui;
}

void MainDebugWindow::init(string mainConfigFile, string guiConfigFile)
{
    // -------- Configure that GUI ----------------------
    configureGUI(guiConfigFile);

    // -------- Instanciate Main Program ----------------
    program = new DenseKitchen;
    this->mainConfigPath = mainConfigFile;
    if(!program->initialize(this->mainConfigPath)){
        LOG("MainDebugWindow initialization error", "program initialization failed");
        debugging::logObject.dumpToConsole();
        exit(-1);
    }

    // -------- Configuration Widget ---------------------
    configurationWindow = new MainConfigurationWindow;
    configurationWindow->initialize(program, "masks.yml");
    connect(this, SIGNAL(updateDebugViews(Frame)),
            configurationWindow, SLOT(updateWindow(Frame)));

    // -------- Configuration Widget ---------------------
    calibrationWindow = new CalibrationWindow;
    calibrationWindow->initialize(program);
    connect(this, SIGNAL(updateDebugViews(Frame)),
            calibrationWindow, SLOT(updateWindow(Frame)));

    // -------- Camera/Step Selector Init ----------------
    cameraItemModel = new QStandardItemModel;
    cameraTree = ui->camerasTreeView;
    cameraTree->setModel(cameraItemModel);

    QStringList headerLabels;
    headerLabels << "Cameras" << "Step";
    cameraItemModel->setHorizontalHeaderLabels(headerLabels);

    cameraSelection = cameraTree->selectionModel();

    connect(cameraSelection, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(cameraSelectionUpdate(QModelIndex,QModelIndex)));

    currentCameraIndex = -1;
    currentKey = "";

    // ------ Log ----------------------------------------
    logItemModel = new QStandardItemModel;
    headerLabels.clear();
    headerLabels << "Time" << "Tag" << "Message" << "CallFunc" << "Line";
    logItemModel->setHorizontalHeaderLabels(headerLabels);


    logProxyModel = new QSortFilterProxyModel;
    logProxyModel->setDynamicSortFilter(true);
    logProxyModel->setSourceModel(logItemModel);

    logTree = ui->logTreeView;
    logTree->setRootIsDecorated(false);
    logTree->setSortingEnabled(true);
    logTree->setModel(logProxyModel);

    logTree->setColumnWidth(0, 80);
    logTree->setColumnWidth(1, 100);
    logTree->setColumnWidth(2, 450);
    logTree->setColumnWidth(3, 100);
    logTree->setColumnWidth(4, 50);

    // ------ Profiler -----------------------------------
    profilerItemModel = new QStandardItemModel;
    headerLabels.clear();
    headerLabels << "Step" << "Time Consumption" << "Average";
    profilerItemModel->setHorizontalHeaderLabels(headerLabels);

    profilerProxyModel = new QSortFilterProxyModel;
    profilerProxyModel->setDynamicSortFilter(true);
    profilerProxyModel->setSourceModel(profilerItemModel);

    profilerTree = ui->profilerTreeView;
    profilerTree->setRootIsDecorated(false);
    profilerTree->setSortingEnabled(true);
    profilerTree->setModel(profilerProxyModel);

    profilerTree->setColumnWidth(0, 150);
    profilerTree->setColumnWidth(1, 80);
    profilerTree->setColumnWidth(2, 80);

    // ------ Logo -------------------------------------
    QImage logo;
    logo.load("GDKmini.png");
    ui->logoLabel->setPixmap(QPixmap::fromImage(logo));

    // ------ Timer -------------------------------------
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGUI()));
    timer->start(timerDelay);
}

void MainDebugWindow::configureGUI(std::string guiConfigFile)
{
    this->guiConfigPath = guiConfigFile;
    if(!readConfig(guiConfigPath)){
        generateConfig(guiConfigPath);
    }

    debugViewGrid = new DebugViewGrid;
    debugViewGrid->initialize(2);
    debugViewGrid->show();

    // Display preset processingsteps
    for(unsigned int i = 0; i < presetCameraNumber.size(); i++){
        popWindow(presetStepName[i], presetCameraNumber[i]);
    }
    presetStepName.clear();
    presetCameraNumber.clear();
}

bool MainDebugWindow::readConfig(std::string filePath)
{

    if(configFile.open(filePath, cv::FileStorage::READ)){
        qDebug("Reading guiConfig file");
        configFile["timerDelay"] >> timerDelay;
        configFile["isRunning"] >> isRunning;
        configFile["autoAdaptLog"] >> autoAdaptLog;
        configFile["autoAdaptProfiler"] >> autoAdaptProfiler;
        configFile["profilerExpandDepth"] >> profilerExpandDepth;
        configFile["popAllWindows"] >> popAllWindows;
        configFile["presetCameraNumber"] >> presetCameraNumber;
        configFile["presetStepName"] >> presetStepName;
        configFile["recordToFiles"] >> isRecordToFiles;

        return true;
    } else {
        return false;
    }
}

void MainDebugWindow::generateConfig(std::string filePath){
    qDebug("Generating guiConfig file");
    configFile.open(filePath, cv::FileStorage::WRITE);
    configFile << "timerDelay" << 10;
    configFile << "isRunning" << false;
    configFile << "autoAdaptLog" << false;
    configFile << "autoAdaptProfiler" << false;
    configFile << "profilerExpandDepth" << 0;
    configFile << "popAllWindows" << true;
    configFile << "presetCameraNumber" << presetCameraNumber;
    configFile << "presetStepName" << presetStepName;
    configFile << "recordToFiles" << isRecordToFiles;
    configFile.release();

    readConfig(filePath);
}

void MainDebugWindow::popWindow(std::string stepKey, int cameraIndex){
    DebugViewWidget* debugView = new DebugViewWidget;
    debugView->setAttribute(Qt::WA_DeleteOnClose);
    debugView->initialize(stepKey,cameraIndex);

    connect(this, SIGNAL(updateDebugViews(Frame)),
            debugView, SLOT(updateView(Frame)));

    connect(debugView, SIGNAL(aboutToClose(std::string)),
            this, SLOT(removeDebugViewWidget(std::string)));

    //Keep track of the debug views
    std::map<std::string, DebugViewWidget *>::iterator debugViewsIter;
    debugViewsIter = debugViews.find(debugView->getIdentifier());
    if (debugViewsIter == debugViews.end() ) {
        // If non of the existing widgets match,
        // pop a new one
        debugViewGrid->addWidget(debugView);
        debugViews[debugView->getIdentifier()] = debugView;
    }

    // It would be nice if one could do this, but when there are no frames
    // the program dies.
    //emit updateDebugViews(program.frames.getCurrent());
}

void MainDebugWindow::updateGuiComponents(){
    emit updateDebugViews(program->getFrames()->getCurrent());
    updateCameraSelector();
    updateLog();
    updateProfiler();
    clearLogObject();
}

void MainDebugWindow::updateCameraSelector()
{
    // Init camera and frame selector treeView
    Frame& currentFrame = program->getFrames()->getCurrent();

    // Remove old tree
    cameraItemModel->removeRows(0, cameraItemModel->rowCount());

    // Build a new tree
    std::vector<CameraObject> cameras = currentFrame.getCameras();
    int nCameras = (int)cameras.size();
    for (int i = 0; i < nCameras; i++){
        QStandardItem* item = new QStandardItem(QString::number(i));
        item->setSelectable(false);

        CameraObject c = cameras[i];
        int rowCounter = 0;
        std::map<std::string,cv::Mat>::iterator stepImage = c.getImages().begin();
        for(; stepImage != c.getImages().end(); ++stepImage){
            QStandardItem* child = new QStandardItem(QString::fromStdString(stepImage->first));
            child->setSelectable(true);
            item->setChild(rowCounter, 1, child);
            rowCounter++;
        }
        cameraItemModel->appendRow(item);
    }
    cameraTree->expandAll();
    cameraTree->setColumnWidth(0, 50);
}

void MainDebugWindow::updateProfiler()
{
    // Recursivly fill the profiler from the logObject
    if (debugging::logObject.profilerSize() > 0){
        debugging::ProfilerEntry profilerEntry = debugging::logObject.popProfiler();
        QStandardItem* parentItem = new QStandardItem(QString::fromStdString(profilerEntry.tag));
        QStandardItem* totalTime  = new QStandardItem(QString::number(profilerEntry.milliseconds));

        if(!profilerEntry.children.empty()){
            updateProfilerChildren(parentItem, profilerEntry.children);
        }

        QList<QStandardItem*> row;
        row << parentItem << totalTime;

        profilerItemModel->removeRow(0);
        profilerItemModel->appendRow(parentItem);
        profilerTree->expandToDepth(profilerExpandDepth);
    }
    if(autoAdaptProfiler){
        adaptColumnsToContent(profilerTree, profilerItemModel);
    }
}

void MainDebugWindow::updateProfilerChildren(QStandardItem* parentItem, std::list<debugging::ProfilerEntry> children)
{
    // The sub-nodes in the logObject need some special treatment
    std::list<debugging::ProfilerEntry>::iterator child = children.begin();

    int rowCounter = 0;

    for( ; child != children.end(); child++){
        QStandardItem* childTag = new QStandardItem(QString::fromStdString(child->tag));
        QStandardItem* childTime = new QStandardItem(QString::number(child->milliseconds));

        if(!child->children.empty()){
            updateProfilerChildren(childTag, child->children);
        }

        parentItem->setChild(rowCounter, 0, childTag);
        parentItem->setChild(rowCounter, 1, childTime);
        rowCounter++;
    }
}

void MainDebugWindow::updateLog()
{
    debugging::LogIterator logEntry;

    // Itterate through the log and append any new events
    logEntry = debugging::logObject.begin();
    for ( ; logEntry != debugging::logObject.end(); logEntry++){
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::fromStdString(logEntry->time));
        row << new QStandardItem(QString::fromStdString(logEntry->tag));
        row << new QStandardItem(QString::fromStdString(logEntry->message));
        row << new QStandardItem(QString::fromStdString(logEntry->callingFunction));
        row << new QStandardItem(QString::fromStdString(logEntry->lineNumber));
        logItemModel->appendRow(row);
    }
    if(autoAdaptLog){
        adaptColumnsToContent(logTree, logItemModel);
    }
}

void MainDebugWindow::clearLogObject()
{
    debugging::logObject.clear();
}

void MainDebugWindow::adaptColumnsToContent(QTreeView *tree, QStandardItemModel *model)
{
    // Make sure all columns are adapted to their content
    int currentColumn = 0;
    int nColumns = model->columnCount();

    while(currentColumn < nColumns){
        tree->resizeColumnToContents(currentColumn);
        currentColumn++;
    }
}

void MainDebugWindow::restart()
{
    // Delete current instance
    delete program;
    delete configurationWindow;
    delete calibrationWindow;

    debugging::logObject.reset();
    logItemModel->removeRows(0, logItemModel->rowCount());

    // Re initialize
    isRecordToFiles = false;
    videoWriter = 0;

    // -------- Instanciate Main Program ----------------
    program = new DenseKitchen;
    if(!program->initialize(this->mainConfigPath)){
        LOG("MainDebugWindow initialization error", "program initialization failed");
        debugging::logObject.dumpToConsole();
        exit(-1);
    }

    // -------- Configuration Widget ---------------------
    configurationWindow = new MainConfigurationWindow;
    configurationWindow->initialize(program, "masks.yml");
    connect(this, SIGNAL(updateDebugViews(Frame)),
            configurationWindow, SLOT(updateWindow(Frame)));

    // -------- Configuration Widget ---------------------
    calibrationWindow = new CalibrationWindow;
    calibrationWindow->initialize(program);
    connect(this, SIGNAL(updateDebugViews(Frame)),
            calibrationWindow, SLOT(updateWindow(Frame)));

    isRunning = false;
    if(program->singleIteration()){
        updateGuiComponents();
    }
}

void MainDebugWindow::updateGUI()
{
    // Update GUI
    if (isRunning && program->singleIteration()){
        updateGuiComponents();
    } else {
        isRunning = false;
    }

    // Debug
    if(isRecordToFiles) {
        if(videoWriter == 0 || !videoWriter[0].isOpened()) {
            // Video recorder
             if(program->getFrames()->size() > 0 && program->getFrames()->getCurrent().getCameras().back().hasImage("rawImage")) {
                 videoWriter = new cv::VideoWriter[program->getFrames()->getCurrent().getCameras().size()*3];
                 for(int c = 0; c+2 < program->getFrames()->getCurrent().getCameras().size()*3; c += 3) {
                     videoWriter[c] = cv::VideoWriter("rawImage"+std::to_string(c/3)+".avi", CV_FOURCC('D','I','V','X'), 30, program->getFrames()->getCurrent().getCameras()[c/3].getImage("rawImage").size());
                     videoWriter[c+1] = cv::VideoWriter("rawColorImage"+std::to_string(c/3)+".avi", CV_FOURCC('D','I','V','X'), 30, program->getFrames()->getCurrent().getCameras()[c/3].getImage("rawColorImage").size());
                     videoWriter[c+2] = cv::VideoWriter("liveSystem"+std::to_string(c/3)+".avi", CV_FOURCC('D','I','V','X'), 30, program->getFrames()->getCurrent().getCameras()[c/3].getImage("debugImage").size());
                 }
             }
        }
        else
        {
            for(int c = 0; c+2 < program->getFrames()->getCurrent().getCameras().size()*3; c += 3) {
                if(program->getFrames()->getCurrent().getCameras()[c/3].hasImage("rawImage"))
                    videoWriter[c].write(program->getFrames()->getCurrent().getCameras()[c/3].getImage("rawImage"));
                if(program->getFrames()->getCurrent().getCameras()[c/3].hasImage("rawColorImage"))
                    videoWriter[c+1].write(program->getFrames()->getCurrent().getCameras()[c/3].getImage("rawColorImage"));
                if(program->getFrames()->getCurrent().getCameras()[c/3].hasImage("debugImage"))
                    videoWriter[c+2].write(program->getFrames()->getCurrent().getCameras()[c/3].getImage("debugImage"));
            }
        }
    }
}

void MainDebugWindow::cameraSelectionUpdate(QModelIndex current, QModelIndex previous)
{
    if (current.column() == 1){
        currentKey = current.data().toString().toStdString();
        currentCameraIndex = current.parent().row();
    }
   emit updateDebugViews(program->getFrames()->getCurrent());
}

void MainDebugWindow::removeDebugViewWidget(std::string identifier)
{
    debugViews.erase(identifier);
}


// ----- EVENTS -----

void MainDebugWindow::keyPressEvent(QKeyEvent * e)
{
    switch(e->key()){
    case Qt::Key_Return:
        if(isRunning){
            isRunning = false;
        } else {
            isRunning = true;
        }
        break;
    case Qt::Key_C:
        configurationWindow->show();
        break;
    case Qt::Key_V:
        calibrationWindow->show();
        break;
    case Qt::Key_F5:
        restart();
        break;
    case Qt::Key_Escape:
        qDebug() << "Escaping";
        this->close();
        break;
    default:
        // Pass event forward
        QMainWindow::keyPressEvent(e);
        break;
    }
}

void MainDebugWindow::closeEvent(QCloseEvent * event)
{
    std::map<std::string,DebugViewWidget*>::iterator debugView;
    for (debugView = debugViews.begin() ; debugView != debugViews.end();++debugView) {
            delete debugView->second;
        }
    delete debugViewGrid;
    delete configurationWindow;
    delete calibrationWindow;
    event->accept();
}


// ----- GUI BUTTONS ETC. -----

void MainDebugWindow::on_timeFilterLineEdit_textEdited(const QString &arg1)
{
    logProxyModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString));
    logProxyModel->setFilterKeyColumn(0);
}

void MainDebugWindow::on_tagFilterLineEdit_textEdited(const QString &arg1)
{
    logProxyModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString));
    logProxyModel->setFilterKeyColumn(1);
}

void MainDebugWindow::on_messageFilterLineEdit_textEdited(const QString &arg1)
{
    logProxyModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString));
    logProxyModel->setFilterKeyColumn(2);
}

void MainDebugWindow::on_callingFuncFilterLineEdit_textEdited(const QString &arg1)
{
    logProxyModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString));
    logProxyModel->setFilterKeyColumn(3);
}

void MainDebugWindow::on_lineNumberFilterLineEdit_textEdited(const QString &arg1)
{
    logProxyModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString));
    logProxyModel->setFilterKeyColumn(4);
}

void MainDebugWindow::on_runButton_clicked()
{
    isRunning = true;
}

void MainDebugWindow::on_pauseButton_clicked()
{
    isRunning = false;
}

void MainDebugWindow::on_stepForwardButton_clicked()
{
    isRunning = false;
    if(program->singleIteration()){
        updateGuiComponents();
    }
}

void MainDebugWindow::on_popWindowButton_clicked()
{
    popWindow(currentKey, currentCameraIndex);
    presetStepName.push_back(currentKey);
    presetCameraNumber.push_back(currentCameraIndex);
}

void MainDebugWindow::on_autoAdaptLogCheckBox_clicked(bool checked)
{
    autoAdaptLog = checked;
    adaptColumnsToContent(logTree, logItemModel);
}

void MainDebugWindow::on_autoAdaptProfilerCheckBox_clicked(bool checked)
{
    autoAdaptProfiler = checked;
    adaptColumnsToContent(profilerTree, profilerItemModel);
}

void MainDebugWindow::on_expandDepthSpinBox_valueChanged(int arg1)
{
    profilerExpandDepth = arg1;
    profilerTree->expandToDepth(profilerExpandDepth);
}

void MainDebugWindow::on_actionClear_triggered()
{
    debugViewGrid->clearGrid();
    debugViews.clear();
}

void MainDebugWindow::on_actionSave_grid_configuration_triggered()
{
    qDebug("Saving grid configuration");
    configFile.open(guiConfigPath, cv::FileStorage::WRITE);
    configFile << "presetCameraNumber" << presetCameraNumber;
    configFile << "presetStepName" << presetStepName;
    configFile.release();
}

void MainDebugWindow::on_actionRun_triggered()
{
    isRunning = true;
}

void MainDebugWindow::on_actionPause_triggered()
{
    isRunning = false;
}

void MainDebugWindow::on_actionRestart_triggered()
{
    restart();
}

void MainDebugWindow::on_actionConfigure_triggered()
{
    configurationWindow->show();
}

void MainDebugWindow::on_actionCalibrate_triggered()
{
    calibrationWindow->show();
}
