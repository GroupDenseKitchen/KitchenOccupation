#include "MainDebugWindow.hpp"
#include "ui_MainDebugWindow.h"
#include <QtWidgets>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include "../Utilities/Frame.hpp"
#include <iostream>


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
    readConfig("guiConfig.yml");

    isRunning = false;
    timerDelay = 10;
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

    Frame currentFrame;
    // Needed to get frames
    program.singleIteration();
    currentFrame = program.frames.getCurrent();

    std::vector<CameraObject> cameras = currentFrame.getCameras();
    int nCameras = cameras.size();

    qDebug(QByteArray::number(nCameras));

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
        }
        cameraItemModel->appendRow(item);
    }
    cameraTree->expandAll();

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

    adaptLogColumnsToContent();

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
    qDebug(QByteArray::number(current.column()));
    qDebug(QByteArray::number(current.row()));
    qDebug(current.data().toByteArray());

    if (current.column() == 1){
        currentKey = current.data().toString().toStdString();
        currentCameraIndex = current.parent().row();
    }
   emit updateDebugViews(program.frames.getCurrent());
}

void MainDebugWindow::readConfig(std::string filePath)
{
    if(configFile.open(filePath, cv::FileStorage::READ)){
        qDebug("reading");
        configFile["timerDelay"] >> timerDelay;
        configFile["isRunning"] >> isRunning;
    } else {
        qDebug("Generate");
        configFile.open(filePath, cv::FileStorage::WRITE);
        configFile << "timerDelay" << 20;
        configFile << "isRunning" << false;
        configFile.release();
    }
}

void MainDebugWindow::adaptLogColumnsToContent()
{
    int currentColumn = 0;
    int nColumns = logItemModel->columnCount();

    while(currentColumn < nColumns){
        logTree->resizeColumnToContents(currentColumn);
        currentColumn++;
    }
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

    adaptLogColumnsToContent();
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
        //profilerTree->expandToDepth(2);
        profilerTree->expandAll();
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
    DebugViewWidget* debugView = new DebugViewWidget;
    debugView->setAttribute(Qt::WA_DeleteOnClose);
    debugView->init(currentKey,currentCameraIndex);

    connect(this, SIGNAL(updateDebugViews(Frame)),
            debugView, SLOT(updateView(Frame)));

    connect(this, SIGNAL(destroyed()),
            debugView, SLOT(close()));

    debugView->show();

    //debugViews.push_back(debugView);
}
