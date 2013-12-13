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

void CalibrationWindow::initialize(DenseKitchen* mainProgram )
{
    this->mainProgram = mainProgram;
    if(mainProgram->getSettings()->hasInt("lowestDistanceOverFloor")){
        thresholdValue = mainProgram->getSettings()->getInt("lowestDistanceOverFloor");
    } else {
        thresholdValue = 255;
    }

    ui->thresholdLabel->setText("Threshold: " + QString::number(thresholdValue));
    ui->thresholdSlider->setValue(thresholdValue);
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

void CalibrationWindow::calculateHistogram()
{
    // Calculate histogram

    cv::Mat histogram;
    int histSize = 320;
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    cv::calcHist(&thresholdedImage,
             1,
             0,
             cv::Mat(),
             histogram,
             1,
             &histSize,
             &histRange);

    int hist_w = thresholdedImage.size().width;
    int hist_h = thresholdedImage.size().height;
    int bin_w = cvRound( (double) hist_w/histSize );
    cv::Mat histImage( hist_h, hist_w, thresholdedImage.type(), cv::Scalar(0) );

    histogram.at<int>(0,0) = 0;
    cv::normalize(histogram,histogram, 0, histImage.rows,cv::NORM_MINMAX, -1, cv::Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1)) ) ,
                             cv::Point( bin_w*(i),   hist_h - cvRound(histogram.at<float>(i)) ),
                             cv::Scalar( 255 ), 2, 8, 0 );
    }
    histogramImage = histImage.clone();

    cv::calcHist(&matImage,
             1,
             0,
             cv::Mat(),
             histogram,
             1,
             &histSize,
             &histRange);

    histImage = cv::Mat( hist_h, hist_w, matImage.type(), cv::Scalar(0) );

    histogram.at<int>(0,0) = 0;
    cv::normalize(histogram,histogram, 0, histImage.rows,cv::NORM_MINMAX, -1, cv::Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1)) ) ,
                             cv::Point( bin_w*(i),   hist_h - cvRound(histogram.at<float>(i)) ),
                             cv::Scalar( 128 ), 2, 8, 0 );
    }
    histogramImage += histImage.clone();
}

void CalibrationWindow::updateGUIImages()
{
    thresholdImage();
    calculateHistogram();

    if(!thresholdedImage.empty()){
        qImage = QImage( thresholdedImage.data,
                         thresholdedImage.cols,
                         thresholdedImage.rows,
                         thresholdedImage.step,
                         QImage::Format_Indexed8);

        ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }
    if(!histogramImage.empty()){
        qImage = QImage( histogramImage.data,
                         histogramImage.cols,
                         histogramImage.rows,
                         histogramImage.step,
                         QImage::Format_Indexed8);

        ui->histogramLabel->setPixmap(QPixmap::fromImage(qImage));
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
        on_applyButton_clicked();
        break;

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
    mainProgram->getSettings()->writeToFile();
    close();
}

void CalibrationWindow::on_thresholdSlider_valueChanged(int value)
{
    thresholdValue = value;
    ui->thresholdLabel->setText("Threshold: " + QString::number(thresholdValue));
    updateGUIImages();
}
