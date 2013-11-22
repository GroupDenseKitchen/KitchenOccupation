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

         // Threshold (remove anything shorter than ... cm)
         cv::threshold(grayImage, grayImage, 130, 50, 4);
         camera.addImage("GrayImage", grayImage);

         // Foreground mask
         cv::Mat foregroundMask = cv::Mat::zeros(grayImage.rows, grayImage.cols, grayImage.type());
         cv::threshold(grayImage, foregroundMask, 1, 255, 0);
             // noise removal
             cv::Mat kernel = cv::Mat::ones(3,3, CV_8U);
                 // closening
                 cv::erode(foregroundMask, foregroundMask, kernel, cv::Point(-1,-1), 4);
                 cv::dilate(foregroundMask, foregroundMask, kernel, cv::Point(-1,-1), 3);
                 // opening
                 cv::dilate(foregroundMask, foregroundMask, kernel, cv::Point(-1,-1), 3);
                 cv::erode(foregroundMask, foregroundMask, kernel, cv::Point(-1,-1), 3);
         camera.addImage("Foreground mask", foregroundMask);


         // Finding "sure" foreground area
         cv::Mat dist_transform;
         cv::distanceTransform(foregroundMask, dist_transform, CV_DIST_L2, 3);
         cv::Mat sure_fg;
         cv::threshold(dist_transform, sure_fg, 0.5*cv::norm(dist_transform, cv::NORM_INF), 255, 0);
         sure_fg.convertTo(sure_fg, CV_8U);
         camera.addImage("sure_fg", sure_fg);


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
