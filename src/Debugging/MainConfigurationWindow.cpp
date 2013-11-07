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
    init();
}

MainConfigurationWindow::~MainConfigurationWindow()
{
    delete ui;
}

void MainConfigurationWindow::init()
{
    filePath = "masks.yml";
}

void MainConfigurationWindow::updateWindow(Frame currentFrame)
{
    int cameraNumber = 0;
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
    doorMask = cv::Mat(640, 480, CV_8UC1);
    exclusionMask = cv::Mat(640, 480, CV_8UC1);
    drawPolygons("doorPolygons", doorPolygons, cv::Scalar(63,232,41));
    drawPolygons("exclusionPolygons", exclusionPolygons, cv::Scalar(255,45,70));
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
        } else if(maskType == "exclusionPolygons"){
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

void MainConfigurationWindow::closeEvent(QCloseEvent * e)
{
    e->accept();
}

void MainConfigurationWindow::mousePressEvent(QMouseEvent *e)
{
    qDebug() << e->pos().x() << " " << e->pos().y();
    polygon.push_back(cv::Point(e->pos().x(), e->pos().y()));

    showImage();
}

void MainConfigurationWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Z:
        if(e->modifiers() == Qt::ControlModifier){
            polygon.pop_back();
            showImage();
        }
        break;
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

void MainConfigurationWindow::on_saveMasksButton_clicked()
{
    configFile.open(filePath, cv::FileStorage::WRITE);
    storeMask(doorPolygons, "doorPolygons");
    storeMask(exclusionPolygons, "exclusionPolygons");
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
    if(configFile.open(filePath, cv::FileStorage::READ)){
        readMasks(doorPolygons, "doorPolygons");
        readMasks(exclusionPolygons, "exclusionPolygons");
    }
    configFile.release();
    showImage();
}

bool MainConfigurationWindow::readMasks(QVector<QVector<cv::Point>> &polygons, std::string nodeName)
{
    std::stringstream ss;
    ss << nodeName << "Count";
    std::vector<int> numberOfPolygons;
    configFile[ss.str()] >> numberOfPolygons;
    std::vector<int> node;
    cv::Point point;

    for(int i = 0; i < numberOfPolygons.size(); i++){
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
}