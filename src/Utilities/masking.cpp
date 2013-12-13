#ifdef HEADLESS
#include "masking.hpp"

namespace masking{

cv::FileStorage configFile;
std::vector<cv::Point> polygon;
std::vector<std::vector<cv::Point> > doorPolygons;
std::vector<std::vector<cv::Point> > exclusionPolygons;
bool drawAsCircle, isDrawingCircle;
cv::Point circleCenter;
int circleRadius;
cv::Mat doorMask, exclusionMask, imageWithMask, checkpointMaskSmall, checkpointMaskMedium, checkpointMaskLarge;

void drawPolygonsToMat(cv::Mat targetMat, const cv::Point **polygonPtrPtr, int numberOfPoints[], cv::Scalar color)
{
    fillPoly( targetMat,
              polygonPtrPtr,
              numberOfPoints,
              1,
              color,
              8 );
}

void drawPolygons(std::string maskType, std::vector<std::vector<cv::Point> > polygons, cv::Scalar color)
{
    for(int i = 0; i < polygons.size(); i++){
        const cv::Point* polygonPtr = polygons[i].data();
        const cv::Point** polygonPtrPtr = &polygonPtr;

        int numberOfPoints[] = {(int)polygons[i].size()};

        //drawPolygonsToMat(imageWithMask, polygonPtrPtr, numberOfPoints, color);

        if(maskType == "doorPolygons"){
            drawPolygonsToMat(doorMask, polygonPtrPtr, numberOfPoints, cv::Scalar(255,255,255));
        } else if(maskType == "exclusionPolygons") {
            drawPolygonsToMat(exclusionMask, polygonPtrPtr, numberOfPoints, cv::Scalar(255,255,255));
        }
    }
}
void drawCheckPointCircles()
{
    cv::circle(checkpointMaskSmall,  circleCenter, circleRadius,       cv::Scalar(255,255,255),-1); //Small
    cv::circle(checkpointMaskMedium, circleCenter, 1.1*circleRadius,   cv::Scalar(255,255,255),-1); //Medium
    cv::circle(checkpointMaskLarge,  circleCenter, 1.2*circleRadius,   cv::Scalar(255,255,255),-1); //Large
}

void drawPolygon(std::vector<cv::Point> polygon, cv::Scalar color)
{
    const cv::Point* polygonPtr = polygon.data();
    const cv::Point** polygonPtrPtr = &polygonPtr;

    int numberOfPoints[] = {(int)polygon.size()};

    drawPolygonsToMat(imageWithMask, polygonPtrPtr, numberOfPoints, color);
}

void drawPolygonsToMasks()
{
    //imageWithMask = matImage.clone();

    doorMask = cv::Mat::zeros(480, 640, CV_8UC3);
    exclusionMask = cv::Mat::zeros(480, 640, CV_8UC3);

    checkpointMaskSmall = cv::Mat::zeros(480, 640, CV_8UC3);
    checkpointMaskMedium = cv::Mat::zeros(480, 640, CV_8UC3);
    checkpointMaskLarge = cv::Mat::zeros(480, 640, CV_8UC3);

    drawCheckPointCircles();
    drawPolygons("exclusionPolygons", exclusionPolygons, cv::Scalar(255,45,70));
    drawPolygons("doorPolygons", doorPolygons, cv::Scalar(63,232,41));
    //drawPolygon(polygon, cv::Scalar(255,218,56));
}


bool readMasks(std::vector<std::vector<cv::Point> > &polygons, std::string nodeName)
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

void loadMaskFromFile()
{

    std::string filePath = "masks.yml";
    if(configFile.open(filePath, cv::FileStorage::READ)){
        readMasks(doorPolygons, "doorPolygons");
        readMasks(exclusionPolygons, "exclusionPolygons");
        configFile["circleCenterX"] >> circleCenter.x;
        configFile["circleCenterY"] >> circleCenter.y;
        configFile["circleRadius"] >> circleRadius;
    }
    configFile.release();
    //sendMasksToFrameList();
}

void sendMasksToFrameList(DenseKitchen* mainProgram)
{
    mainProgram->getFrames()->setDoorMask(doorMask);
    mainProgram->getFrames()->setExclusionMask(exclusionMask);
    mainProgram->getFrames()->setCheckPointMaskSmall(checkpointMaskSmall);
    mainProgram->getFrames()->setCheckPointMaskMedium(checkpointMaskMedium);
    mainProgram->getFrames()->setCheckPointMaskLarge(checkpointMaskLarge);

    cv::Mat inclusionMask;
    inclusionMask.create(exclusionMask.rows, exclusionMask.cols, exclusionMask.type());
    inclusionMask.zeros(exclusionMask.rows, exclusionMask.cols, exclusionMask.type());
    cv::bitwise_not(exclusionMask, inclusionMask);

    mainProgram->getFrames()->setInclusionMask(inclusionMask);
}

}
#endif