#include "MainDebugWindow.hpp"
#include "ui_MainDebugWindow.h"
#include <QtWidgets>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include "../Utilities/Frame.hpp"


MainDebugWindow::MainDebugWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDebugWindow)
{
    ui->setupUi(this);


}

MainDebugWindow::~MainDebugWindow()
{
    delete ui;
}

void MainDebugWindow::configureGUI()
{
    // TODO
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
        std::map<std::string,cv::Mat>::iterator stepImage = c.images.begin();
        for(; stepImage != c.images.end(); ++stepImage){
            QStandardItem* child = new QStandardItem(QString::fromStdString(stepImage->first));
            child->setSelectable(true);
            item->setChild(0, 1, child);
        }
        cameraItemModel->appendRow(item);
    }

    // ------ Log ---------------------------------------

    logItemModel = new QStandardItemModel;
    headerLabels.clear();
    headerLabels << "Time" << "Tag" << "Message" << "CallFunc" << "Line";
    logItemModel->setHorizontalHeaderLabels(headerLabels);


    logProxyModel = new QSortFilterProxyModel;
    logProxyModel->setSourceModel(logItemModel);

    logTree = ui->logTreeView;
    logTree->setRootIsDecorated(false);
    logTree->setSortingEnabled(true);
    logTree->setModel(logProxyModel);

    adaptLogColumnsToContent();

    // ------ Timer -------------------------------------
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGUI()));
    timer->start(timerDelay);
}

void MainDebugWindow::updateGUI()
{
    // Update GUI
    if (isRunning && program.singleIteration()){
        updateDebugViews();
        updateLog();
    } else {
        isRunning = false;
    }
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
    updateDebugViews();
}

void MainDebugWindow::updateDebugViews()
{
    if(currentKey.length() > 0 && currentCameraIndex != -1){
        cv::Mat matImage = program.frames.getCurrent().getCameras()[currentCameraIndex].images[currentKey];
        cv::cvtColor(matImage, matImage, CV_BGR2RGB);

        qImage = QImage((uchar*)matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGB888);
        ui->image1->setPixmap(QPixmap::fromImage(qImage));
    }
}

void MainDebugWindow::readConfig(std::string filePath)
{
    qDebug("Reading Config");

    //configFile.open(filePath, cv::FileStorage::READ);

    if(configFile.open("filePath.yml", cv::FileStorage::READ)){
        // TODO: Read config
        qDebug("Seccesfull reading");
        configFile["isRunning"] >> isRunning;

    } else {
        // TODO: Generate default config
    }

    /*
    configFile.open("filePath", cv::FileStorage::WRITE);
    configFile << "timerDelay" << 20;
    configFile << "isRunning" << false;
    configFile.release();
    */
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
        updateDebugViews();
    }
}

void MainDebugWindow::on_stepBackwardButton_clicked()
{
    isRunning = false;
    // TODO
}

void MainDebugWindow::on_tagFilterLineEdit_textEdited(const QString &arg1)
{
    logProxyModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString));
    logProxyModel->setFilterKeyColumn(1);
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

    debugging::logObject.clear();
}
