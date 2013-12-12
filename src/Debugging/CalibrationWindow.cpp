#include "CalibrationWindow.hpp"
#include "ui_CalibrationWindow.h"

#include <QDebug>

CalibrationWindow::CalibrationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);
    setWindowTitle("Calibration Window");
    thresholdValue = ui->thresholdSlider->value();
}

CalibrationWindow::~CalibrationWindow()
{
    delete ui;
}

void CalibrationWindow::initialize(DenseKitchen* mainProgram ,std::string filepath)
{
    this->mainProgram = mainProgram;
    this->filePath = filepath;
    loadMaskFromFile();

}

void CalibrationWindow::updateWindow(Frame currentFrame)
{
    unsigned int cameraNumber = 0;
    std::string processStep = "rawImage";

    if ( cameraNumber < currentFrame.getCameras().size() ) {
        CameraObject cam = currentFrame.getCameras()[cameraNumber];
        if (cam.hasImage(processStep)) {
            storeImageLocally(cam.getImage(processStep));
            updateGUIImages();
        } else {
             LOG("DebugViewWidget Error", "Process step doesn't exist in DebugView::updateView");
        }
    } else {
         LOG("DebugViewWidget Error", "Camera doesn't exist in DebugView::updateView");
    }
}

void CalibrationWindow::storeImageLocally(const cv::Mat &image)
{
    if (!image.isContinuous()) {
        LOG("DebugViewWidget Error", "cv::Mat is not continuous");
    }

    switch (image.type()) {
    case CV_8UC1:
        matImage = image.clone();
        break;
    case CV_8UC3:
        cv::cvtColor(image, matImage, CV_BGR2GRAY );
        break;
    default :
        LOG("DebugViewWidget Error", "Unexpected CV image format");
        break;
    }
}

void CalibrationWindow::thresholdImage()
{
    cv::threshold(matImage, thresholdedImage, thresholdValue, 255, 4);
}

void CalibrationWindow::updateGUIImages()
{
    thresholdImage();

    if(!thresholdedImage.empty()){
        qImage = QImage( thresholdedImage.data,
                         thresholdedImage.cols,
                         thresholdedImage.rows,
                         thresholdedImage.step,
                         QImage::Format_Indexed8);

        ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }
}

void CalibrationWindow::loadMaskFromFile()
{

}

void CalibrationWindow::closeEvent(QCloseEvent * e)
{
    e->accept();
}

void CalibrationWindow::mousePressEvent(QMouseEvent *e)
{
    int x = e->pos().x();
    int y = e->pos().y();

}

void CalibrationWindow::mouseMoveEvent(QMouseEvent *e)
{

}

void CalibrationWindow::mouseReleaseEvent(QMouseEvent *e)
{

}

void CalibrationWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Z:
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:

    default:
        break;
    }
}



bool CalibrationWindow::readMasks(QVector<QVector<cv::Point>> &polygons, std::string nodeName)
{
    return true;
}

void CalibrationWindow::on_cancelButton_clicked()
{
    close();
}

void CalibrationWindow::on_applyButton_clicked()
{
    mainProgram->getSettings()->setInt("lowestDistanceOverFloor", thresholdValue);

    close();
}

void CalibrationWindow::on_thresholdSlider_valueChanged(int value)
{
    thresholdValue = value;
    updateGUIImages();
}
