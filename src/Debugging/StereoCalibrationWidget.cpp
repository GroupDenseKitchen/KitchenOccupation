#include "StereoCalibrationWidget.hpp"
#include "ui_StereoCalibrationWidget.h"

StereoCalibrationWidget::StereoCalibrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StereoCalibrationWidget)
{
    ui->setupUi(this);
    setWindowTitle("Cali-mother-fucking-brate");
}

StereoCalibrationWidget::~StereoCalibrationWidget()
{
    delete ui;
}

void StereoCalibrationWidget::showImage()
{
    if(!matImage.empty()){
        QImage qImage = QImage( matImage.data,
                         matImage.cols,
                         matImage.rows,
                         matImage.step,
                         QImage::Format_RGB888);

        ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }
}

void StereoCalibrationWidget::updateWindow(Frame currentFrame)
{
    // TODO: revieve frame and extract cameras and images;
    unsigned int cameraNumber = 0;
    std::string processStep = "rawImage";

    if ( cameraNumber < currentFrame.getCameras().size() ) {
        CameraObject cam = currentFrame.getCameras()[cameraNumber];
        if (cam.hasImage(processStep)) {
            storeImage(cam.getImage(processStep));
            showImage();
        } else {
             LOG("DebugViewWidget Error", "Process step doesn't exist in DebugView::updateView");
        }
    } else {
         LOG("DebugViewWidget Error", "Camera doesn't exist in DebugView::updateView");
    }
}

void StereoCalibrationWidget::storeImage(const cv::Mat &image)
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

void StereoCalibrationWidget::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void StereoCalibrationWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
    case Qt::Key_Escape:
        close();
        break;

    }
}
