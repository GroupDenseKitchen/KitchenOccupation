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

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGUI()));
    timerDelay = 20;
    timer->start(20);
}

MainDebugWindow::~MainDebugWindow()
{
    delete ui;
}

void MainDebugWindow:: init()
{
    program.initialize("dense_conf.yml");

    // Init camera and frame selector treeView
    Frame currentFrame;
    // Needed to get frames
    program.singleIteration();  // WARNING: returns false if it fails!!! TODO: handle error!
    currentFrame = program.frames.getCurrent();

    std::vector<CameraObject> cameras = currentFrame.getCameras();
    int nCameras = cameras.size();

    qDebug("%d",nCameras);

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

    isRunning = false;
}

void MainDebugWindow::updateGUI()
{
    // Update GUI
    if (isRunning && program.singleIteration()){
        updateDebugViews();
    } else {
        isRunning = false;
    }
}

void MainDebugWindow::cameraSelctionUpdate(QModelIndex current, QModelIndex previous)
{
    qDebug("%d",current.column());
    qDebug("%d",current.row());
    qDebug("%s",current.data().toString().toStdString().c_str());

    currentCameraIndex = current.parent().row();

    if (current.column() == 1){
        currentKey = current.data().toString().toStdString();
        cv::Mat matImage = program.frames.getCurrent().getCameras()[currentCameraIndex].images[currentKey];
        cv::cvtColor(matImage, matImage, CV_BGR2RGB);

        qImage = QImage((uchar*)matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGB888);
        ui->image1->setPixmap(QPixmap::fromImage(qImage));
    }
}

void MainDebugWindow:: updateDebugViews()
{
    if(currentKey.length() > 0 && currentCameraIndex != -1){
        cv::Mat matImage = program.frames.getCurrent().getCameras()[currentCameraIndex].images[currentKey];
        cv::cvtColor(matImage, matImage, CV_BGR2RGB);

        qImage = QImage((uchar*)matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGB888);
        ui->image1->setPixmap(QPixmap::fromImage(qImage));
    }
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
