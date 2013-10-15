#include "BackgroundModel.hpp"

namespace image_processing
{

BackgroundModel::BackgroundModel(){

}
BackgroundModel::~BackgroundModel(){

}

bool BackgroundModel::initialize(configuration::ConfigurationManager &conf)
{
    isInitialized = true;

    // Initialize variables
    //
    //           |VARIABLE          |NAME                  |DEFAULT
    //---------------------------------------------------------------
    CONFIG(conf, nmixtures,         "nmixtures",            5);
    CONFIG(conf, backgroundRatio,   "backgroundRatio",      0.9);
    CONFIG(conf, varThresholdGen,   "varThresholdGen",      19);
    CONFIG(conf, fVarInit,          "fVarInit",             15);
    CONFIG(conf, fCT,               "fCT",                  0.05);
    CONFIG(conf, isShadowDetection, "isShadowDetection",    false);
    CONFIG(conf, erotions,          "erotions",             3);
    CONFIG(conf, dilations,         "dilations",            3);
    CONFIG(conf, history,           "history",              500);
    //fVarMin = ;
    //fVarMax = ;
    //fTau = ;

    bg.set("history",history);
    bg.set("varThreshold",varThresholdGen);
    //bg.setBool("shadowDetection",isShadowDetection);
    bg.set("nmixtures",nmixtures);
    bg.set("backgroundRatio",backgroundRatio);

    //TODO: more parameters should be set, see above and clean it up, function should get inparameters...

    return isInitialized;
}

 void BackgroundModel::process(FrameList &frames)
 {
     //TODO: loop over all cameras...
     CameraObject &camera = frames.getCurrent().getCameras().back();

     if(!camera.hasImage("rawImage"))
     {
         LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
         return;
     }

     cv::Mat rawImage = camera.getImage("rawImage");
     cv::Mat foregroundMask;    // From frames (current)

     bg(rawImage,foregroundMask,0.001);
     cv::erode(foregroundMask,foregroundMask,cv::Mat(),cv::Point(-1,-1), erotions);
     cv::dilate(foregroundMask,foregroundMask,cv::Mat(),cv::Point(-1,-1), dilations);
     cv::threshold(foregroundMask,foregroundMask,230,255,CV_THRESH_BINARY);
     cv::namedWindow("binary");
     cv::imshow("binary", foregroundMask);


     camera.addImage("foregroundMask", foregroundMask);
 }

}
