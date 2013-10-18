#include "MainDebugWindow.hpp"
#include "ui_MainDebugWindow.h"
#include <QtWidgets>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include "../Utilities/Frame.hpp"
#include <iostream>

#include <QString>

#include <QString>


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
    configPath = "guiConfig0_3.yml";
    if(!readConfig(configPath)){
        generateConfig(configPath);
    }
}

void MainDebugWindow::init()
{
    // --------------------------------------------------
    configureGUI();

    // -------- Instanciate Main Program ----------------
    program.readConfig("dense_conf.yml");
    program.init();

    // -------- Camera/Step Selector Init ---------------
    cameraItemModel = new QStandardItemModel;
    cameraTree = ui->camerasTreeView;
    cameraTree->setModel(cameraItemModel);

    QStringList headerLabels;
    headerLabels << "Cameras" << "Step";
    cameraItemModel->setHorizontalHeaderLabels(headerLabels);

    cameraSelection = cameraTree->selectionModel();

    connect(cameraSelection, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(cameraSelctionUpdate(QModelIndex,QModelIndex)));

    currentCameraIndex = -1;
    currentKey = "";

    // Init camera and frame selector treeView
    Frame currentFrame;
    // Needed to get frames
    program.singleIteration();  // WARNING: returns false if it fails!!! TODO: handle error!
    currentFrame = program.frames.getCurrent();

    std::vector<CameraObject> cameras = currentFrame.getCameras();
    int nCameras = cameras.size();

    for (int i = 0; i < nCameras; i++){
        QStandardItem* item = new QStandardItem(QString::number(i));
        item->setSelectable(false);

        CameraObject c = cameras[i];
        int rowCounter = 0;
        std::map<std::string,cv::Mat>::iterator stepImage = c.images.begin();
        for(; stepImage != c.images.end(); ++stepImage){
            QStandardItem* child = new QStandardItem(QString::fromStdString(stepImage->first));
            child->setSelectable(true);
            item->setChild(rowCounter, 1, child);
            rowCounter++;
            /*
            if (popAllWindows){
                std::string childKey = child->data().toString().toStdString();
                int childCameraIndex = i;
                popWindow(childKey, childCameraIndex);
            }
            */
        }
        cameraItemModel->appendRow(item);
    }
    cameraTree->expandAll();
    cameraTree->setColumnWidth(0, 50);

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
    if (isRunning && program.singleIteration()){
        updateGuiComponents();
        //emit updateDebugViews(program.frames.getCurrent());
        //updateLog();
    } else {
        isRunning = false;
    }
}

void MainDebugWindow::updateGuiComponents(){
    emit updateDebugViews(program.frames.getCurrent());
    updateLog();
    updateProfiler();
    clearLogObject();
}

void MainDebugWindow::cameraSelctionUpdate(QModelIndex current, QModelIndex previous)
{
    if (current.column() == 1){
        currentKey = current.data().toString().toStdString();
        currentCameraIndex = current.parent().row();
    }
   emit updateDebugViews(program.frames.getCurrent());
}

bool MainDebugWindow::readConfig(std::string filePath)
{
    if(configFile.open(filePath, cv::FileStorage::READ)){
        qDebug("reading");
        configFile["timerDelay"] >> timerDelay;
        configFile["isRunning"] >> isRunning;
        configFile["autoAdaptLog"] >> autoAdaptLog;
        configFile["autoAdaptProfiler"] >> autoAdaptProfiler;
        configFile["profilerExpandDepth"] >> profilerExpandDepth;
        configFile["popAllWindows"] >> popAllWindows;
        return true;
    } else {
        return false;
    }
}

void MainDebugWindow::generateConfig(std::string filePath){
    qDebug("Generating");
    configFile.open(filePath, cv::FileStorage::WRITE);
    configFile << "timerDelay" << 10;
    configFile << "isRunning" << false;
    configFile << "autoAdaptLog" << false;
    configFile << "autoAdaptProfiler" << false;
    configFile << "profilerExpandDepth" << 0;
    configFile << "popAllWindows" << true;
    configFile.release();

    readConfig(filePath);
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
    if(program.singleIteration()){
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
}

void MainDebugWindow::popWindow(std::string stepKey, int cameraIndex){
    DebugViewWidget* debugView = new DebugViewWidget;
    debugView->setAttribute(Qt::WA_DeleteOnClose);
    debugView->init(currentKey,currentCameraIndex);

    connect(this, SIGNAL(updateDebugViews(Frame)),
            debugView, SLOT(updateView(Frame)));

    connect(debugView, SIGNAL(aboutToClose(std::string)),
            this, SLOT(removeDebugViewWidget(std::string)));

    debugView->show();

    //Keep track of the debug views
    std::map<std::string, DebugViewWidget *>::iterator debugViewsIter;
    debugViewsIter = debugViews.find(debugView->getIdentifier());
    if (debugViewsIter != debugViews.end() ) {
        debugViewsIter->second->close();
        debugViews[debugView->getIdentifier()] = debugView;
    } else {
        debugViews[debugView->getIdentifier()] = debugView;
    }

    emit updateDebugViews(program.frames.getCurrent());
}

void MainDebugWindow::closeEvent(QCloseEvent * event)
{
    std::map<std::string,DebugViewWidget*>::iterator debugView;
    for (debugView = debugViews.begin() ; debugView != debugViews.end();++debugView) {
            delete debugView->second;
        }
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
