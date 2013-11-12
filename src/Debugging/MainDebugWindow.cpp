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
}

MainDebugWindow::~MainDebugWindow()
{
    delete ui;
}

void MainDebugWindow::configureGUI()
{
    guiConfigPath = "guiConfig.yml";
    if(!readConfig(guiConfigPath)){
        generateConfig(guiConfigPath);
    }

    debugViewGrid = new DebugViewGrid;
    debugViewGrid->init(2);
    debugViewGrid->showMaximized();

    // Display preset processingsteps
    for(unsigned int i = 0; i < presetCameraNumber.size(); i++){
        popWindow(presetStepName[i], presetCameraNumber[i]);
    }
    presetStepName.clear();
    presetCameraNumber.clear();
}

void MainDebugWindow::init()
{
    // --------------------------------------------------
    configureGUI();

    // -------- Instanciate Main Program ----------------
    program = new DenseKitchen;
    mainConfigPath = "dense_conf.yml";
    if(!program->initialize(mainConfigPath)){
        // TODO Fix that shit
    }

    // -------- Instanciate Main Program ----------------
    configWindow = new MainConfigurationWindow;
    configWindow->init(program, "masks.yml");
    connect(this, SIGNAL(updateDebugViews(Frame)),
            configWindow, SLOT(updateWindow(Frame)));

    // -------- Camera/Step Selector Init ---------------
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

    // ------ Log ---------------------------------------

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

    // ------ Profiler ----------------------------------

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

    // ------ Timer -------------------------------------
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGUI()));
    timer->start(timerDelay);
}

void MainDebugWindow::updateGUI()
{
    // Update GUI
    if (isRunning && program->singleIteration()){
        updateGuiComponents();
    } else {
        isRunning = false;
    }
}

void MainDebugWindow::updateGuiComponents(){
    emit updateDebugViews(program->frames.getCurrent());
    updateCameraSelector();
    updateLog();
    updateProfiler();
    clearLogObject();
}

void MainDebugWindow::cameraSelectionUpdate(QModelIndex current, QModelIndex previous)
{
    if (current.column() == 1){
        currentKey = current.data().toString().toStdString();
        currentCameraIndex = current.parent().row();
    }
   emit updateDebugViews(program->frames.getCurrent());
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
    configFile.release();

    readConfig(filePath);
}

void MainDebugWindow::updateCameraSelector()
{
    // Init camera and frame selector treeView
    Frame& currentFrame = program->frames.getCurrent();

    // Remove old tree
    cameraItemModel->removeRows(0, cameraItemModel->rowCount());

    std::vector<CameraObject> cameras = currentFrame.getCameras();
    int nCameras = cameras.size();
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

void MainDebugWindow::updateLog()
{
    debugging::LogIterator logEntry;

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

void MainDebugWindow::updateProfiler()
{
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

void MainDebugWindow::updateProfilerChildren(QStandardItem* parentItem, std::list<debugging::ProfilerEntry> children){
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

void MainDebugWindow::clearLogObject()
{
    debugging::logObject.clear();
}

void MainDebugWindow::adaptColumnsToContent(QTreeView *tree, QStandardItemModel *model)
{
    int currentColumn = 0;
    int nColumns = model->columnCount();

    while(currentColumn < nColumns){
        tree->resizeColumnToContents(currentColumn);
        currentColumn++;
    }
}

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

void MainDebugWindow::on_stepBackwardButton_clicked()
{
    isRunning = false;
    // TODO
}

void MainDebugWindow::on_popWindowButton_clicked()
{
    popWindow(currentKey, currentCameraIndex);
    presetStepName.push_back(currentKey);
    presetCameraNumber.push_back(currentCameraIndex);
}

void MainDebugWindow::popWindow(std::string stepKey, int cameraIndex){
    DebugViewWidget* debugView = new DebugViewWidget;
    debugView->setAttribute(Qt::WA_DeleteOnClose);
    debugView->init(stepKey,cameraIndex);

    connect(this, SIGNAL(updateDebugViews(Frame)),
            debugView, SLOT(updateView(Frame)));

    connect(debugView, SIGNAL(aboutToClose(std::string)),
            this, SLOT(removeDebugViewWidget(std::string)));

    //Keep track of the debug views
    std::map<std::string, DebugViewWidget *>::iterator debugViewsIter;
    debugViewsIter = debugViews.find(debugView->getIdentifier());
    if (debugViewsIter != debugViews.end() ) {
        // If the widget allready exists, do nothing
        //debugViewsIter->second->close();
        //debugViews[debugView->getIdentifier()] = debugView;
    } else {
        debugViewGrid->addWidget(debugView);
        debugViews[debugView->getIdentifier()] = debugView;
    }

    //emit updateDebugViews(program.frames.getCurrent());
}

void MainDebugWindow::restart()
{
    delete program;
    debugging::logObject.reset();

    logItemModel->removeRows(0, logItemModel->rowCount());

    program = new DenseKitchen;
    program->initialize(mainConfigPath);
    if(program->singleIteration()){
        updateGuiComponents();
    }
}

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
    delete configWindow;
    event->accept();
}

void MainDebugWindow::removeDebugViewWidget(std::string identifier)
{
    debugViews.erase(identifier);
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

void MainDebugWindow::on_configureButton_clicked()
{
    configWindow->show();
}

void MainDebugWindow::on_actionClear_triggered()
{
    debugViewGrid->clearGrid();
    debugViews.clear();
}

void MainDebugWindow::on_actionSave_grid_configuration_triggered()
{
    qDebug("Generating guiConfig file");
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
    configWindow->show();
}
