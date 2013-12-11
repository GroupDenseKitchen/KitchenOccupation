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
    CONFIG(settings,   headHeightMargin,  "headHeightMargin",     7);
    CONFIG(settings,   lowestDistanceOverFloor,  "lowestDistanceOverFloor",     90);
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

         // Invert and threshold => Darker is further away and black is unkown or thresholded out pixels
         cv::Mat correctedGrayImage = 255-grayImage;
         cv::threshold(correctedGrayImage, correctedGrayImage, 255-distanceToCameraMargin, 255, 4);
         camera.addImage("correctedGrayImage", correctedGrayImage);

         // Find contours
         std::vector<std::vector<cv::Point> > contours;
         std::vector<cv::Vec4i> hierarchy;
         cv::findContours( correctedGrayImage.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

         // Draw contours (debugging)
         cv::Mat contourMat = correctedGrayImage.clone();
         for( int i = 0; i < contours.size(); i++ ) {
             if(contourArea(contours[i]) > minimalHumanArea) {
                 drawContours( contourMat, contours, i, cv::Scalar(255), 2, 8, hierarchy, 0, cv::Point() );
             }
         }
         camera.addImage("contourMat",contourMat);

         // Generate a binary mask of all maximal regions (a nicer foreground map)
         cv::Mat debugImage = correctedGrayImage.clone();
         cv::Mat maxRegionMask = cv::Mat::zeros(correctedGrayImage.size(), correctedGrayImage.type());
         generateMaxRegionMask(maxRegionMask, correctedGrayImage, contours, minimalHumanArea, debugImage);
         camera.addImage("maxRegionMask", maxRegionMask);
         camera.addImage("MaxRegionDebug", debugImage);


         // Detect individual humans (mostly their heads) and give each a random color. (debugging)
         cv::findContours( maxRegionMask.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
         cv::RNG rng(12345);
         cv::Mat individualHumans = cv::Mat::zeros(correctedGrayImage.size(), CV_8UC3);
         for( int i = 0; i < contours.size(); i++ )
         {
             double area = cv::contourArea(contours[i]);
             if(area > minimalHeadArea) {
                 drawContours( individualHumans, contours, i, cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), -1, 8, hierarchy, 0, cv::Point() );

                 // Calculate center of mass
                 cv::Moments mu = cv::moments(contours[i], true);
                 cv::Point2f centerOfMass = cv::Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
                 cv::Rect boundingBox = cv::boundingRect(contours[i]);
                 camera.getObjects().push_back(Object(contours[i], boundingBox, centerOfMass, area));
             }
         }
         camera.addImage("individualHumans", individualHumans);



         // Calculate histogram
         cv::Mat histogram;
         int histSize = 256;
         float range[] = { 0, 256 } ;
         const float* histRange = { range };
         cv::calcHist(&grayImage,
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

         cv::normalize(histogram, histogram, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

         for( int i = 1; i < histSize; i++ )
         {
             cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1)) ) ,
                                  cv::Point( bin_w*(i),   hist_h - cvRound(histogram.at<float>(i)) ),
                                  cv::Scalar( 255 ), 2, 8, 0 );
         }
         camera.addImage("histImage", histImage);


         n++;
     }
 }

void KinectSegmentation::generateMaxRegionMask(cv::Mat & maxRegionMask, cv::Mat & grayImage, std::vector<std::vector<cv::Point>> & contours, int minimalArea, cv::Mat & debugImage) {

    // Generate binary contour masks (must have an area of atleast minimalHumanArea)
    std::vector<cv::Mat> contourMasks;
    for( int i = 0; i < contours.size(); i++ ) {
        if(contourArea(contours[i]) > minimalArea) {
            cv::Mat contourMask(maxRegionMask.size(), maxRegionMask.type(), cv::Scalar(0));
            drawContours( contourMask, contours, i, cv::Scalar(255), -1);
            contourMasks.push_back(contourMask);

            // Debug
            drawContours( debugImage, contours, i, cv::Scalar(255), 2);
        }
    }

    // Remove compression artifacts and noise.
    cv::erode(grayImage, grayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 4);
    cv::dilate(grayImage, grayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 4);
    cv::dilate(grayImage, grayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 4);
    cv::erode(grayImage, grayImage, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 5);
    cv::GaussianBlur(grayImage, grayImage, cv::Size(3,3), 1);

    // Find maximum in each contour and generate a maximum binary map (maxRegionMat) for the entire image.
    double maxValue;
    cv::Point maxPoint;
    for(int n = 0; n < contourMasks.size(); n++) {
        cv::minMaxLoc(grayImage,0,&maxValue,0,&maxPoint,contourMasks[n]);
        cv::Mat maxRegion = grayImage.mul(contourMasks[n], 1.0/255);
        cv::threshold(maxRegion, maxRegion, maxValue-headHeightMargin, 255, 3);
        maxRegionMask += maxRegion;

        // Debug
        cv::circle(debugImage, maxPoint, 15, cv::Scalar(255), 2);
        //LOG("maxValue", n << ": " << maxValue);
    }
    cv::dilate(maxRegionMask, maxRegionMask, cv::Mat::ones(3,3, CV_8U), cv::Point(-1,-1), 3);
}




} // namespace image_processing
