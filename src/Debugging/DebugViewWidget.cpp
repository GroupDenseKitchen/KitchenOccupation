#include "DebugViewWidget.hpp"
#include "ui_DebugViewWidget.h"
#include <QDebug>

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

    //debugImageWidget = new CvImageWidget(this);
    //ui->verticalLayout->addWidget(debugImageWidget);
}

void DebugViewWidget::showImage(const cv::Mat& image)
{
    //debugImageWidget->showImage(image);

    bool useNewCvMat = false;

    switch (image.type()) {
    case CV_8UC1:
        cv::cvtColor(image, matImage, CV_GRAY2RGB );
        qImage = QImage( matImage.data,
                         matImage.cols,
                         matImage.rows,
                         matImage.step,
                         QImage::Format_RGB888);
        useNewCvMat = true;
        break;
    case CV_8UC3:
        cv::cvtColor(image, matImage, CV_BGR2RGB );
        qImage = QImage( matImage.data,
                         matImage.cols,
                         matImage.rows,
                         matImage.step,
                         QImage::Format_Indexed8);
        useNewCvMat = true;
        break;
    default :
        qDebug() << "Unexpected CV image format";
        break;
    }

    if (!qImage.isNull()){
        ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }

}

void DebugViewWidget::updateView( Frame currentFrame)
{
    if ( cameraNumber < currentFrame.getCameras().size() ) {
        CameraObject cam = currentFrame.getCameras()[cameraNumber];
        if (cam.hasImage(processStep)) {
            //debugImageWidget->showImage(cam.getImage(processStep));
            showImage(cam.getImage(processStep));
        } else {
            //TODO: use LOG here instead?
            qDebug() << "Process step doesn't exist in DebugView::updateView";
        }
    } else {
        qDebug() << "Camera doesn't exist in DebugView::updateView";
    }
}
