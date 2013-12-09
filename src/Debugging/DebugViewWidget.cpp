#include "DebugViewWidget.hpp"
#include "ui_DebugViewWidget.h"
#include <QDebug>
#include <QCloseEvent>
#include "../Utilities/Algorithm.hpp"

DebugViewWidget::DebugViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugViewWidget)
{
    ui->setupUi(this);
    windowWidth = ui->imageLabel->width();
    windowHeight = ui->imageLabel->height();
    windowRatio = (float)windowWidth/windowHeight;

    TAG = "debugGUI";
}

DebugViewWidget::~DebugViewWidget()
{
    delete ui;
}

void DebugViewWidget:: initialize(const std::string processStepName, int camNumber)
{
    processStep = processStepName;
    cameraNumber = camNumber;
    QString processStepNameQ = processStep.c_str();
    setWindowTitle("Camera: " + QString::number(cameraNumber) +
                   " Process Step: " + processStepNameQ);
    ui->label->setText("Camera: " + QString::number(cameraNumber) +
                       " Process Step: " + processStepNameQ);
}

void DebugViewWidget::showImage()
{
    if(!resizedImage.empty()){
        qImage = QImage( resizedImage.data,
                         resizedImage.cols,
                         resizedImage.rows,
                         resizedImage.step,
                         QImage::Format_RGB888);

        ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }
}

std::string DebugViewWidget::getIdentifier( )
{
     return std::to_string(cameraNumber) + processStep;
}

void DebugViewWidget::updateView( Frame currentFrame)
{
    if ( cameraNumber < currentFrame.getCameras().size() ) {
        CameraObject cam = currentFrame.getCameras()[cameraNumber];
        if (cam.hasImage(processStep)) {
            storeImage(cam.getImage(processStep));
            adaptImageToWidget();
            showImage();
        } else {
             LOG("DebugViewWidget Error", "Process step doesn't exist in DebugView::updateView");
        }
    } else {
         LOG("DebugViewWidget Error", "Camera doesn't exist in DebugView::updateView");
    }
}

void DebugViewWidget::storeImage(const cv::Mat &image)
{
    if (!image.isContinuous()) {
        LOG("DebugViewWidget Error", "cv::Mat is not continuous");
    }

    switch (image.type()) {
    case CV_8UC1:
        cv::cvtColor(image, matImage, CV_GRAY2RGB );
        break;
    case CV_8UC3:
        cv::cvtColor(image, matImage, CV_BGR2RGB );
        break;
    default :
        LOG("DebugViewWidget Error", "Unexpected CV image format");
        break;
    }
}

void DebugViewWidget::adaptImageToWidget()
{
    if(!matImage.empty()){
        float trueRatio = (float) matImage.cols/matImage.rows;

        if(windowRatio > trueRatio){
            // Window is too wide
            int correctedWidth = (int)ceil( trueRatio * windowHeight );
            cv::resize(matImage, resizedImage, cv::Size(correctedWidth, windowHeight));
        } else {
            // Window is too heigh
            int correctedHeight = (int)ceil( (float)windowWidth / trueRatio );
            cv::resize(matImage, resizedImage, cv::Size(windowWidth, correctedHeight));
        }
    }
}

void DebugViewWidget::closeEvent(QCloseEvent * event)
{
    emit aboutToClose(this->getIdentifier());
    event->accept();
}

void DebugViewWidget::resizeEvent(QResizeEvent *e){
    windowWidth = e->size().width();
    windowHeight = e->size().height();
    windowRatio = (float) windowWidth/windowHeight;

    adaptImageToWidget();
    showImage();

    e->accept();
}
