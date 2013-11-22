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
         cv::Mat maskedImage;

         if(frames.hasInclusionMask()){
            cv::bitwise_and(rawImage, frames.getInclusionMask(), rawImage);
         }

         cv::Mat grayImage;
         cv::cvtColor(rawImage, grayImage, CV_BGR2GRAY);

         /*
         // Normalize image
         if(maxIntensity == 0){
             maxIntensity = cv::norm(grayImage, cv::NORM_INF);
             qDebug() << "MaxIntens: " << maxIntensity;
         }
         grayImage *= 255.0/maxIntensity;
*/
         // Threshold
         cv::threshold(grayImage, grayImage, 130, 50, 4);


         camera.addImage("GrayImage", grayImage);

/*

         cv::Mat histogram;
         int histSize = 256;
         int histWidth = rawImage.size().width;
         int histHeight = rawImage.size().height;
         float range[] = {0, 256};
         const float* histRange = &range[0];
         cv::calcHist(&grayImage, 1, 0, cv::Mat(), histogram, 1,  &histSize, &histRange);

         // Remove undefined areas
         histogram.at<int>(0) = 0;
         histogram.at<int>(1) = 0;
         histogram.at<int>(2) = 0;
         histogram.at<int>(3) = 0;

         cv::normalize( histogram, histogram, 0, histogram.rows, cv::NORM_MINMAX, -1, cv::Mat() );

         if(histogram.empty()){
             qDebug() << "hist is empty";
         } else {
             qDebug() << "Hist dimensions: " << histogram.size().height << ", " << histogram.size().width;
         }

         int binW = cvRound( (double) histWidth/histSize);


         cv::Mat histImage( histHeight, histWidth, CV_8UC3, cv::Scalar(0,0,0) );

         for(int i = 0; i < histSize; i++){
             cv::line( histImage, cv::Point(binW * (i-1), histHeight - cvRound(histogram.at<float>(i-1))),
                                  cv::Point(binW * (i), histHeight - cvRound(histogram.at<float>(i))),
                       cv::Scalar(255,255,255), 2, 8, 0);
         }

         camera.addImage("Histogram", histImage);
*/

         cv::Mat test(grayImage.rows, grayImage.cols, grayImage.type());
         test = cv::Mat::zeros(grayImage.rows, grayImage.cols, grayImage.type());


         //cv::threshold(grayImage, grayImage, 1, 255, 0);

    // noise removal
    cv::Mat opening;
    cv::Mat kernel = cv::Mat::ones(3,3, CV_8U);
    //cv::morphologyEx(grayImage, opening, cv::MORPH_OPEN, kernel, cv::Point(-1,-1), 4);

    opening = grayImage.clone();
    cv::erode(opening, opening, kernel, cv::Point(-1,-1), 2);
    cv::dilate(opening, opening, kernel, cv::Point(-1,-1), 2);

    cv::dilate(opening, opening, kernel, cv::Point(-1,-1), 2);
    cv::erode(opening, opening, kernel, cv::Point(-1,-1), 2);

    std::vector<cv::Point2i> minimas;
    cv::Mat minimaImage;
    findMinimas(grayImage, minimaImage, minimas);
    camera.addImage("minima", minimaImage);

/*
    // sure background area
    cv::Mat sure_bg;
    cv::dilate(opening, sure_bg, kernel, cv::Point(-1,-1), 3);
*/
    // Finding sure foreground area
    cv::Mat dist_transform;
    cv::distanceTransform(opening, dist_transform, CV_DIST_L2, 3);
    cv::Mat sure_fg;
    cv::threshold(dist_transform, sure_fg, 0.7*cv::norm(dist_transform, cv::NORM_INF), 255, 0);


    // Finding unknown region
    sure_fg.convertTo(sure_fg, CV_8U);
    /*
    cv::Mat unknown;
    cv::subtract(sure_bg, sure_fg, unknown);
    */

    camera.addImage("opening", opening);
    //camera.addImage("sure_bg", sure_bg);
    camera.addImage("sure_fg", sure_fg);
    //camera.addImage("unknown", unknown);

         /*
         cv::vector<cv::vector<cv::Point> > contours;
         std::vector<cv::Vec4i> hierarchy;
         cv::findContours( grayImage.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
         for( int i = 0; i < contours.size(); i++ )
         {
             cv::Scalar color = cv::Scalar( 255, 0, 0 );
             double area = contourArea(contours[i]);
             if(contours[i].size() > 120 && area > 500)
                 drawContours( test, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
         }

         camera.addImage("test", test);
    */



         //camera.addImage("foregroundMask", foregroundMask);
         n++;
     }
 }

using namespace std;
void findMinimas(cv::Mat & src, cv::Mat & dst, std::vector<cv::Point2i> & minimas) {

    dst = cv::Mat::zeros(src.size(), src.type());
    for(int x = 1; x < src.size().height-1; x++)
        for(int y = 1; y < src.size().width-1; y++) {
            uchar c = src.at<uchar>(x,y);
            uchar lu = src.at<uchar>(x-1,y+1);
            uchar l = src.at<uchar>(x-1,y);
            uchar ld = src.at<uchar>(x-1,y-1);
            uchar u = src.at<uchar>(x,y+1);
            uchar d = src.at<uchar>(x,y-1);
            uchar ru = src.at<uchar>(x+1,y+1);
            uchar r = src.at<uchar>(x+1,y);
            uchar rd = src.at<uchar>(x+1,y-1);

            if(c != 0 && c <= max(lu,max(l,max(ld,max(u,max(d,max(ru,max(r,rd))))))))
                dst.at<uchar>(x,y) = 255;
        }
}


} // namespace image_processing
