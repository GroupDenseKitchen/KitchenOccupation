#include "BackgroundModelMoG.hpp"

namespace image_processing
{

BackgroundModelMoG::BackgroundModelMoG(){

}
BackgroundModelMoG::~BackgroundModelMoG(){

}

bool BackgroundModelMoG::initialize(configuration::ConfigurationManager &settings)
{
    isInitialized = true;

    // Initialize variables
    //
    //           |VARIABLE          |NAME                  |DEFAULT
    //---------------------------------------------------------------
    CONFIG(settings, nmixtures,         "nmixtures",            5);
    CONFIG(settings, backgroundRatio,   "backgroundRatio",      0.9);
    CONFIG(settings, varThresholdGen,   "varThresholdGen",      30);
    CONFIG(settings, fVarInit,          "fVarInit",             15);
    CONFIG(settings, fCT,               "fCT",                  0.05);
    CONFIG(settings, isShadowDetection, "isShadowDetection",    false);
    CONFIG(settings, erotions,          "erotions",             3);
    CONFIG(settings, dilations,         "dilations",            3);
    CONFIG(settings, history,           "history",              500);

    return isInitialized;
}

 void BackgroundModelMoG::process(FrameList &frames)
 {
     for(CameraObject & camera : frames.getCurrent().getCameras())
     {
         if(!camera.hasImage("rawImage"))
         {
             LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
             return;
         }

         cv::Mat rawImage = camera.getImage("rawImage");
         cv::Mat foregroundMask;    // From frames (current)

         //TODO this value 0.001 should be a configurable variable
         bg(rawImage,foregroundMask,0.001);
         cv::erode(foregroundMask,foregroundMask,cv::Mat(),cv::Point(-1,-1), erotions);
         cv::dilate(foregroundMask,foregroundMask,cv::Mat(),cv::Point(-1,-1), dilations);
         cv::threshold(foregroundMask,foregroundMask,230,255,CV_THRESH_BINARY);
         //cv::namedWindow("binary");
         //cv::imshow("binary", foregroundMask);

         camera.addImage("foregroundMask", foregroundMask);
     }
 }

}
