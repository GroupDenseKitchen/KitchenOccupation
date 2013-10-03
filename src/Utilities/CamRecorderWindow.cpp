#include "CamRecorderWindow.h"
#include "../Forms/CamRecorderWindow.ui"
#include <iostream>

CamRecorderWindow::CamRecorderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CamRecorderWindow)
{
    fpsMeter.start();
    ui->setupUi(this);

    // ------------- SETUP CAMERAS -------------------

    //cam0 = new cv::VideoCapture(0);

    // http://USERNAME:PASSWORD@IP-ADRESS/video.cgi?INTERNAL_SETTINGS&.mjpg
    // &.mjpg at the end to help OpenCV find the right codec
    cam0 = new cv::VideoCapture("http://root:pass@169.254.192.68/axis-cgi/mjpg/video.cgi?resolution=640x480&.mjpg");


    cam1 = new cv::VideoCapture(1);
    cam2 = new cv::VideoCapture(2);

    // ------------- ADD CAMERAS TO GUI ---------------

    if(cam0->isOpened()){
        ui->camComboBox->addItem("Cam0", QVariant(0));
    } else {
        qWarning("Could not open Cam0");
    }
    if(cam1->isOpened()){
        ui->camComboBox->addItem("Cam1", QVariant(1));
    } else {
        qWarning("Could not open Cam1");
    }
    if(cam2->isOpened()){
        ui->camComboBox->addItem("Cam2", QVariant(2));
    } else {
        qWarning("Could not open Cam2");
    }

    // --------------------------------------------------

    camNumber = 0;

    record = false;

    // Settings
    fps = 30;
    ui->fpsValue->setText(QString::number(fps));
    ui->fpsSlider->setValue(fps);
    color = true;
    ui->colorCheckBox->setChecked(true);
    flip = false;


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(camUpdate()));
    timer->start(50);

    nanoSec = fpsMeter.nsecsElapsed();
    qDebug(QByteArray::number(nanoSec));
}

CamRecorderWindow::~CamRecorderWindow()
{
    delete ui;
    cam0->release();
    cam1->release();
    cam2->release();
}

void CamRecorderWindow::camUpdate(){
    fpsMeter.start();
    //Grab frame from cam and see if it works
    switch(camNumber){
    case 0:
        cam0->read(matOriginal);
        break;
    case 1:
        cam1->read(matOriginal);
        break;
    case 2:
        cam2->read(matOriginal);
        break;
    default:
        qDebug("No Cam");
    }

    if (matOriginal.empty()) return;

    if(record){
        videoWriter.write(matOriginal);
    }

    if(color){
        cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);
        qImgOriginal = QImage((uchar*) matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    } else {
        cv::cvtColor(matOriginal, matOriginal, CV_BGR2GRAY);
        qImgOriginal = QImage((uchar*) matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_Indexed8);
    }

    cv::flip(matOriginal, matOriginal, 1);

    ui->imageLbl->setPixmap(QPixmap::fromImage(qImgOriginal));

    nanoSec = fpsMeter.nsecsElapsed();
    qDebug("NanoSecs: " + QByteArray::number(nanoSec));
}

void CamRecorderWindow::keyPressEvent(QKeyEvent * event){
    if (event->key() == Qt::Key_Space){
        record = !record;
    }
}

void CamRecorderWindow::on_recordBtn_clicked()
{
    if(videoWriter.isOpened()){
        record = true;
    } else {
        if(cam0->isOpened()){
            cv::Size size = matOriginal.size();
            //double camfps = cam0->get(CV_CAP_PROP_FPS);          // The fps is not really achieved
            //qDebug("FPS:" + QByteArray::number(camfps));
            //double codec = cam.get(CV_CAP_PROP_FOURCC);     // The achieved codec doesn't work
            //bool color = true;
            videoWriter.open("videoSave.avi", CV_FOURCC_DEFAULT, 10, size, color);
            if(videoWriter.isOpened()){
                record = true;
                qDebug("Video Writer just opened");
            } else {
                qWarning("The Video Stream could not be opened");
            }
        } else {
            qWarning("Your cam is not working");
        }
    }
}

void CamRecorderWindow::on_stopBtn_clicked()
{
    record = false;
    videoWriter.release();
}

void CamRecorderWindow::on_fpsSlider_sliderMoved(int position)
{
    //fps = position;
    ui->fpsValue->setText(QString::number(fps));
}

void CamRecorderWindow::on_colorCheckBox_clicked(bool checked)
{
    color = checked;
}

void CamRecorderWindow::on_flipCheckBox_clicked(bool checked)
{
    flip = checked;
}

void CamRecorderWindow::on_camComboBox_activated(int index)
{
    camNumber = ui->camComboBox->itemData(index).toInt();
    qDebug(QByteArray::number(camNumber));
}
