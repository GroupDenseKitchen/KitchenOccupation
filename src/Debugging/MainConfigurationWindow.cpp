#include "MainConfigurationWindow.hpp"
#include "ui_MainConfigurationWindow.h"

#include <QDebug>

MainConfigurationWindow::MainConfigurationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainConfigurationWindow)
{
    ui->setupUi(this);
    //setWindowModality(Qt::ApplicationModal);
    //setWindowFlags( Qt::WindowStaysOnTopHint );
}

MainConfigurationWindow::~MainConfigurationWindow()
{
    delete ui;
}

void MainConfigurationWindow::init(DenseKitchen* _mainProgram ,std::string _filepath)
{
    mainProgram = _mainProgram;
    filePath = _filepath;
    isDrawingCircle = false;
    circleRadius = 0;
    circleCenter = cv::Point(0,0);

    loadMaskFromFile();
    applyChanges();
    showImage();
}

void MainConfigurationWindow::applyChanges()
{
    mainProgram->frames.setDoorMask(doorMask);
    mainProgram->frames.setExclusionMask(exclusionMask);
    mainProgram->frames.setCheckPointMaskSmall(checkpointMaskSmall);
    mainProgram->frames.setCheckPointMaskMedium(checkpointMaskMedium);
    mainProgram->frames.setCheckPointMaskLarge(checkpointMaskLarge);

    cv::Mat inclusionMask;
    inclusionMask = cv::Mat::zeros(exclusionMask.rows, exclusionMask.cols, exclusionMask.type());
    cv::bitwise_not(exclusionMask, inclusionMask);

    //cv::imshow("", inclusionMask);

    mainProgram->frames.setInclusionMask(inclusionMask);

    close();
}

void MainConfigurationWindow::updateWindow(Frame currentFrame)
{
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

void MainConfigurationWindow::showImage()
{
    overlayMask();

    if(!imageWithMask.empty()){
        qImage = QImage( imageWithMask.data,
                         imageWithMask.cols,
                         imageWithMask.rows,
                         imageWithMask.step,
                         QImage::Format_RGB888);

        ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }
    if(!doorMask.empty()){
        resizedImage = cv::Mat::zeros(320, 240, CV_8UC3);
        cv::resize(doorMask, resizedImage, cv::Size(320, 240));
        qImage = QImage( resizedImage.data,
                         resizedImage.cols,
                         resizedImage.rows,
                         resizedImage.step,
                         QImage::Format_RGB888);

        ui->doorMaskLabel->setPixmap(QPixmap::fromImage(qImage));
    }
    if(!exclusionMask.empty()){
        resizedImage = cv::Mat::zeros(320, 240, CV_8UC3);
        cv::resize(exclusionMask, resizedImage, cv::Size(320, 240));
        qImage = QImage( resizedImage.data,
                         resizedImage.cols,
                         resizedImage.rows,
                         resizedImage.step,
                         QImage::Format_RGB888);

        ui->exclusionMaskLabel->setPixmap(QPixmap::fromImage(qImage));
    }
}

void MainConfigurationWindow::storeImage(const cv::Mat &image)
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

void MainConfigurationWindow::overlayMask()
{
    imageWithMask = matImage.clone();

    doorMask = cv::Mat::zeros(480, 640, CV_8UC3);
    exclusionMask = cv::Mat::zeros(480, 640, CV_8UC3);

    checkpointMaskSmall = cv::Mat::zeros(480, 640, CV_8UC3);
    checkpointMaskMedium = cv::Mat::zeros(480, 640, CV_8UC3);
    checkpointMaskLarge = cv::Mat::zeros(480, 640, CV_8UC3);

    drawCheckPointCircles();
    drawPolygons("exclusionPolygons", exclusionPolygons, cv::Scalar(255,45,70));
    drawPolygons("doorPolygons", doorPolygons, cv::Scalar(63,232,41));
    drawPolygon(polygon, cv::Scalar(255,218,56));
}

void MainConfigurationWindow::drawPolygons(std::string maskType, QVector<QVector<cv::Point>> polygons, cv::Scalar color)
{
    for(int i = 0; i < polygons.size(); i++){
        const cv::Point* polygonPtr = polygons[i].constData();
        const cv::Point** polygonPtrPtr = &polygonPtr;

        int numberOfPoints[] = {polygons[i].size()};

        polygonDrawer(imageWithMask, polygonPtrPtr, numberOfPoints, color);

        if(maskType == "doorPolygons"){
            polygonDrawer(doorMask, polygonPtrPtr, numberOfPoints, cv::Scalar(255,255,255));
        } else if(maskType == "exclusionPolygons") {
            polygonDrawer(exclusionMask, polygonPtrPtr, numberOfPoints, cv::Scalar(255,255,255));
        }
    }
}

void MainConfigurationWindow::drawPolygon(QVector<cv::Point> polygon, cv::Scalar color)
{
    const cv::Point* polygonPtr = polygon.constData();
    const cv::Point** polygonPtrPtr = &polygonPtr;

    int numberOfPoints[] = {polygon.size()};

    polygonDrawer(imageWithMask, polygonPtrPtr, numberOfPoints, color);
}

void MainConfigurationWindow::polygonDrawer(cv::Mat targetMat, const cv::Point **polygonPtrPtr, int numberOfPoints[], cv::Scalar color)
{
    fillPoly( targetMat,
              polygonPtrPtr,
              numberOfPoints,
              1,
              color,
              8 );
}

void MainConfigurationWindow::drawCheckPointCircles()
{
    cv::circle(imageWithMask, circleCenter, 1.2*circleRadius, cv::Scalar(149,255,78), 2); //Large
    cv::circle(imageWithMask, circleCenter, 1.1*circleRadius, cv::Scalar(255,213,83), 2); //Medium
    cv::circle(imageWithMask, circleCenter, circleRadius,     cv::Scalar(255,123,83), 2); //Small

    cv::circle(checkpointMaskSmall,  circleCenter, circleRadius,       cv::Scalar(255,255,255),-1); //Small
    cv::circle(checkpointMaskMedium, circleCenter, 1.1*circleRadius,   cv::Scalar(255,255,255),-1); //Medium
    cv::circle(checkpointMaskLarge,  circleCenter, 1.2*circleRadius,   cv::Scalar(255,255,255),-1); //Large
}

void MainConfigurationWindow::loadMaskFromFile()
{
    if(configFile.open(filePath, cv::FileStorage::READ)){
        readMasks(doorPolygons, "doorPolygons");
        readMasks(exclusionPolygons, "exclusionPolygons");
        configFile["circleCenterX"] >> circleCenter.x;
        configFile["circleCenterY"] >> circleCenter.y;
        configFile["circleRadius"] >> circleRadius;
        applyChanges();
    }
    configFile.release();
}

void MainConfigurationWindow::closeEvent(QCloseEvent * e)
{
    e->accept();
}

void MainConfigurationWindow::mousePressEvent(QMouseEvent *e)
{
    int x = e->pos().x();
    int y = e->pos().y();

    // Check if we are in the image and correct if else
    int xBoundary = matImage.cols - 1;
    int yBoundary = matImage.rows - 1;

    if(x < 0){
        x = 0;
    }
    if(x > xBoundary){
        x = xBoundary;
    }
    if(y < 0){
        y = 0;
    }
    if(y > yBoundary){
        y = yBoundary;
    }

    // Draw specifyed object
    if(drawAsCircle){
        circleRadius = 1;
        circleCenter = cv::Point(x,y);
        isDrawingCircle = true;
    } else {
        polygon.push_back(cv::Point(x, y));
    }

    showImage();
}

void MainConfigurationWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(isDrawingCircle){
        int x = e->pos().x();
        int y = e->pos().y();

        // Check if we are in the image and correct if else
        int xBoundary = matImage.cols - 1;
        int yBoundary = matImage.rows - 1;

        if(x < 0){
            x = 0;
        }
        if(x > xBoundary){
            x = xBoundary;
        }
        if(y < 0){
            y = 0;
        }
        if(y > yBoundary){
            y = yBoundary;
        }

        cv::Vec2i radiusVector = cv::Point(x,y) - circleCenter;
        circleRadius = cv::norm(radiusVector);
        showImage();
    }
}

void MainConfigurationWindow::mouseReleaseEvent(QMouseEvent *e)
{
    isDrawingCircle = false;
}

void MainConfigurationWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Z:
        if(e->modifiers() == Qt::ControlModifier){
            if(!polygon.empty()){
                polygon.pop_back();
                showImage();
            }
        }
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

void MainConfigurationWindow::on_newPolygonButton_clicked()
{
    polygon.clear();
    showImage();
}

void MainConfigurationWindow::on_addAsDoorButton_clicked()
{
    doorPolygons.push_back(polygon);
    polygon.clear();
    showImage();
}

void MainConfigurationWindow::on_addAsExclusionButton_clicked()
{
    exclusionPolygons.push_back(polygon);
    polygon.clear();
    showImage();
}

void MainConfigurationWindow::on_addAsCheckpointButton_clicked()
{

}

void MainConfigurationWindow::on_saveMasksButton_clicked()
{
    configFile.open(filePath, cv::FileStorage::WRITE);
    storeMask(doorPolygons, "doorPolygons");
    storeMask(exclusionPolygons, "exclusionPolygons");
    configFile << "circleCenterX" << circleCenter.x;
    configFile << "circleCenterY" << circleCenter.y;
    configFile << "circleRadius" << circleRadius;
    configFile.release();
}

void MainConfigurationWindow::storeMask(QVector<QVector<cv::Point>> polygons, std::string nodeName)
{
    polygon.clear();
    std::vector<int> numberOfPolygons;
    std::stringstream ss;
    for(int i = 0; i < polygons.size(); i++){
        polygon = polygons[i];
        numberOfPolygons.push_back(polygon.size());

        for(int j = 0; j <  polygon.size(); j++){
            ss.str("");
            ss << nodeName << i << j;
            configFile << ss.str() << polygon[j];
        }
    }

    ss.str("");
    ss << nodeName << "Count";
    configFile << ss.str() << numberOfPolygons;
}

void MainConfigurationWindow::on_loadMasksButton_clicked()
{
    /*
    if(configFile.open(filePath, cv::FileStorage::READ)){
        readMasks(doorPolygons, "doorPolygons");
        readMasks(exclusionPolygons, "exclusionPolygons");
    }
    configFile.release();
    showImage();
    */
    loadMaskFromFile();
}

bool MainConfigurationWindow::readMasks(QVector<QVector<cv::Point>> &polygons, std::string nodeName)
{
    std::stringstream ss;
    ss << nodeName << "Count";
    std::vector<int> numberOfPolygons;
    configFile[ss.str()] >> numberOfPolygons;
    std::vector<int> node;
    cv::Point point;

    for(unsigned int i = 0; i < numberOfPolygons.size(); i++){
        polygon.clear();

        for(int j = 0; j < numberOfPolygons[i]; j++){
            ss.str("");
            ss << nodeName << i << j;

            configFile[ss.str()] >> node;

            if(node.size() == 2){
                point = cv::Point(node[0], node[1]);
                polygon.push_back(point);
            }
        }

        polygons.push_back(polygon);
        polygon.clear();
    }

    return true;
}

void MainConfigurationWindow::on_clearAllButton_clicked()
{
    doorPolygons.clear();
    doorMask.create(640, 480, CV_8UC3);
    doorMask.zeros(640, 480, CV_8UC3);
    exclusionPolygons.clear();
    exclusionMask.create(640, 480, CV_8UC3);
    exclusionMask.zeros(640, 480, CV_8UC3);
    polygon.clear();
    showImage();
}

void MainConfigurationWindow::on_cancelButton_clicked()
{
    close();
}

void MainConfigurationWindow::on_applyButton_clicked()
{
    applyChanges();
}

void MainConfigurationWindow::on_circleCheckBox_clicked(bool checked)
{
    drawAsCircle = checked;
}
