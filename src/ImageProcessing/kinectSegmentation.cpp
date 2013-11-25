#include "KinectSegmentation.hpp"
#include <QDebug>

namespace image_processing
{

KinectSegmentation::KinectSegmentation(){

}
KinectSegmentation::~KinectSegmentation(){

}

bool KinectSegmentation::initialize(configuration::ConfigurationManager &settings)
{
    isInitialized = true;
    maxIntensity = 0;


    // Initialize variables
    //
    //               |VARIABLE          |NAME                  |DEFAULT
    //---------------------------------------------------------------
    //CONFIG(settings, nmixtures,         "nmixtures",            8);
    CONFIG(settings,   headHeightMargin,  "headHeightMargin",     8);
    CONFIG(settings,   lowestDistanceOverFloor,  "lowestDistanceOverFloor",     110);
    CONFIG(settings,   distanceToCameraMargin,  "distanceToCameraMargin",     15);
    CONFIG(settings,   minimalHumanArea,    "minimalHumanArea",     50*50);
    CONFIG(settings,   minimalHeadArea,     "minimalHeadArea",      15*15);

    return isInitialized;
}

void KinectSegmentation::process(FrameList &frames)
 {
     int n = 0;
     for(CameraObject & camera : frames.getCurrent().getCameras())
     {
         if(!camera.hasImage("rawImage"))
         {
             LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
             return;
         }

         cv::Mat rawImage = camera.getImage("rawImage");

         if(frames.hasInclusionMask()){
            cv::bitwise_and(rawImage, frames.getInclusionMask(), rawImage);
         }

         // Threshold (remove anything shorter than ... cm)
         cv::Mat grayImage;
         cv::cvtColor(rawImage, grayImage, CV_BGR2GRAY);
         cv::threshold(grayImage, grayImage, lowestDistanceOverFloor, 255, 4);
         camera.addImage("GrayImage", grayImage);

         // Invert and threshold => Darker is further away and black is unkown or thresholded out
         cv::Mat correctedGrayImage = 255-grayImage;
         cv::threshold(correctedGrayImage, correctedGrayImage, 255-distanceToCameraMargin, 255, 4);
         camera.addImage("correctedGrayImage", correctedGrayImage);


         // Find contours
         std::vector<std::vector<cv::Point> > contours;
         std::vector<cv::Mat> contourMasks;
         std::vector<cv::Vec4i> hierarchy;
         cv::Mat contourMat = correctedGrayImage.clone();
         cv::findContours( contourMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
         contourMat = correctedGrayImage.clone();
         for( int i = 0; i < contours.size(); i++ )
         {
             if(contourArea(contours[i]) > minimalHumanArea) {
                 drawContours( contourMat, contours, i, cv::Scalar(255), 2, 8, hierarchy, 0, cv::Point() );
                 cv::Mat contourMask(contourMat.size(), contourMat.type(), cv::Scalar(0));
                 drawContours( contourMask, contours, i, cv::Scalar(255), -1, 8, hierarchy, 0, cv::Point() );
                 contourMasks.push_back(contourMask);
             }
         }
         camera.addImage("contourMat",contourMat);

         // Remove compression artifacts and noise.
         cv::erode(correctedGrayImage, correctedGrayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 4);
         cv::dilate(correctedGrayImage, correctedGrayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 4);
         cv::dilate(correctedGrayImage, correctedGrayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 4);
         cv::erode(correctedGrayImage, correctedGrayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 5);

         cv::GaussianBlur(correctedGrayImage, correctedGrayImage, cv::Size(3,3), 1);

         // Calculate histograms
         /*
         cv::Mat histogram;
         int histSize = 256;
         float range[] = { lowestDistanceOverFloor, 256 } ;
         const float* histRange = { range };
         cv::calcHist(&correctedGrayImage,
                  1,
                  0,
                  cv::Mat(),
                  histogram,
                  1,
                  &histSize,
                  &histRange);

         int hist_w = grayImage.size().width;
         int hist_h = grayImage.size().height;
         int bin_w = cvRound( (double) hist_w/histSize );
         cv::Mat histImage( hist_h, hist_w, grayImage.type(), cv::Scalar(0) );
         for( int i = 1; i < histSize; i++ )
         {
             cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1)) ) ,
                                  cv::Point( bin_w*(i),   hist_h - cvRound(histogram.at<float>(i)) ),
                                  cv::Scalar( 255 ), 2, 8, 0 );
             camera.addImage("histImage", histImage);
         }*/


         // Find maximum
         double maxValue;
         cv::Point maxPoint;
         std::vector<cv::Mat> maxRegions;
         cv::Mat maxRegionMat = cv::Mat::zeros(correctedGrayImage.size(), correctedGrayImage.type());
         for(int n = 0; n < contourMasks.size(); n++) {
             cv::minMaxLoc(correctedGrayImage,0,&maxValue,0,&maxPoint,contourMasks[n]);
             cv::circle(contourMat, maxPoint, 15, cv::Scalar(255));
             //LOG("maxValue", n << ": " << maxValue);
             cv::Mat maxRegion = correctedGrayImage.mul(contourMasks[n], 1.0/255);
             cv::threshold(maxRegion, maxRegion, maxValue-headHeightMargin, 255, 3);
             maxRegionMat += maxRegion;
         }
         cv::dilate(maxRegionMat, maxRegionMat, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 3);
         camera.addImage("maxRegions", maxRegionMat);

         // Detect individual humans
         cv::Mat individualHumans = cv::Mat::zeros(correctedGrayImage.size(), CV_8UC3);
         cv::findContours( maxRegionMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
         cv::RNG rng(12345);
         for( int i = 0; i < contours.size(); i++ )
         {
             if(contourArea(contours[i]) > minimalHeadArea) {
                 drawContours( individualHumans, contours, i, cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), -1, 8, hierarchy, 0, cv::Point() );
             }
         }
         camera.addImage("individualHumans", individualHumans);


         n++;
     }
 }

using namespace std;
void findMinimas(cv::Mat & src, cv::Mat & dst, std::vector<cv::Point2i> & minimas) {
    //dst = cv::Mat::zeros(src.size(), src.type());
    dst = src.clone();
    cv::Point extremeLocation(0,0);

    // The neighbourhood too look in must be atleast 3x3 (meaning that we look at least 1 pixel away)
    int windowSize = 11;
    if(windowSize % 2 == 0) {
        LOG("findMinimas error", "windowSize must be >= 3 and ODD, not '" << windowSize << "'");
        return;
    }
    int windowCenter = (windowSize-1)/2;

    // Create a local mask
    cv::Mat localWindowMask = cv::Mat::zeros(cv::Size(windowSize,windowSize),CV_8U);
    localWindowMask.at<unsigned char>(windowCenter,windowCenter)=1;

    cv::Mat localImage;
    for(int x = windowCenter; x < src.size().height-windowCenter; x++)
        for(int y = windowCenter; y < src.size().width-windowCenter; y++) {
            if( dst.at<uchar>(x,y) != 0 ) {
                localImage = dst.colRange(y-windowCenter,y+windowCenter+1).rowRange(x-windowCenter,x+windowCenter+1);
                cv::minMaxLoc(localImage,0,0,&extremeLocation,0);
                if(extremeLocation.x == windowCenter && extremeLocation.y == windowCenter) {
                    localImage = localImage.mul(localWindowMask);
                    //y += windowCenter;
                }
            }
        }
}


} // namespace image_processing
