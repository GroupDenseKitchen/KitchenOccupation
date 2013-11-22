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

         // Normalize image
         if(maxIntensity == 0){
             maxIntensity = cv::norm(grayImage, cv::NORM_INF);
             qDebug() << "MaxIntens: " << maxIntensity;
         }
         grayImage *= 255.0/maxIntensity;

         // Threshold
         cv::threshold(grayImage, grayImage, 150, 0, 4);

         camera.addImage("GrayImage", grayImage);


         cv::Mat histogram;
         int histSize = 256;
         int histWidth = rawImage.size().width;
         int histHeight = rawImage.size().height;
         float range[] = {0, 256};
         const float* histRange = &range[0];
         cv::calcHist(&grayImage, 1, 0, cv::Mat(), histogram, 1,  &histSize, &histRange);

         // Remove undefined areas
         histogram.at<int>(0) = 0;

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






         //camera.addImage("foregroundMask", foregroundMask);
         n++;
     }
 }



} // namespace image_processing
