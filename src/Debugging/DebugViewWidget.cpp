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
}

DebugViewWidget::~DebugViewWidget()
{
    delete ui;
}

void DebugViewWidget:: init(const std::string processStepName, int camNumber)
{
    processStep = processStepName;
    cameraNumber = camNumber;
    QString processStepNameQ = processStep.c_str();
    setWindowTitle("Camera: " + QString::number(cameraNumber) +
                   " Process Step: " + processStepNameQ);
}

void DebugViewWidget::showImage(const cv::Mat& image)
{
    if (!image.isContinuous()) {
        LOG("DebugViewWidget Error", "cv::Mat is not continuous");
    }

    switch (image.type()) {
    case CV_8UC1:
        cv::cvtColor(image, matImage, CV_GRAY2RGB );
        qImage = QImage( matImage.data,
                         matImage.cols,
                         matImage.rows,
                         matImage.step,
                         QImage::Format_RGB888);
        break;
    case CV_8UC3:
        cv::cvtColor(image, matImage, CV_BGR2RGB );
        qImage = QImage( matImage.data,
                         matImage.cols,
                         matImage.rows,
                         matImage.step,
                         QImage::Format_RGB888);
        break;
    default :
        LOG("DebugViewWidget Error", "Unexpected CV image format");
        break;
    }

    ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
}

void DebugViewWidget::updateView( Frame currentFrame)
{
    if ( cameraNumber < currentFrame.getCameras().size() ) {
        CameraObject cam = currentFrame.getCameras()[cameraNumber];
        if (cam.hasImage(processStep)) {
            showImage(cam.getImage(processStep));
        } else {
             LOG("DebugViewWidget Error", "Process step doesn't exist in DebugView::updateView");
        }
    } else {
         LOG("DebugViewWidget Error", "Camera doesn't exist in DebugView::updateView");
    }
}

std::string DebugViewWidget::getIdentifier( )
{
     return std::to_string(cameraNumber) + processStep;
}

void DebugViewWidget::closeEvent(QCloseEvent * event)
{
    emit aboutToClose(this->getIdentifier());
    event->accept();
}
