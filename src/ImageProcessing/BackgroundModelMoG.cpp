#include "BackgroundModelMoG.hpp"

namespace image_processing
{

BackgroundModelMoG::BackgroundModelMoG(){
    bg = 0;
}
BackgroundModelMoG::~BackgroundModelMoG(){

}

bool BackgroundModelMoG::initialize(configuration::ConfigurationManager &settings)
{
    isInitialized = true;

    // Initialize variables
    //
    //               |VARIABLE          |NAME                  |DEFAULT
    //---------------------------------------------------------------
    CONFIG(settings, nmixtures,         "nmixtures",            8);
    CONFIG(settings, backgroundRatio,   "backgroundRatio",      0.9);
    CONFIG(settings, varThresholdGen,   "varThresholdGen",      15);
    CONFIG(settings, varThreshold,      "varThreshold",          16);
    CONFIG(settings, fVarInit,          "fVarInit",             15);
    CONFIG(settings, fCT,               "fCT",                  0.05);
    CONFIG(settings, isShadowDetection, "isShadowDetection",    true);
    CONFIG(settings, erotions,          "erotions",             1);
    CONFIG(settings, dilations,         "dilations",            1);
    CONFIG(settings, history,           "history",              500);
    CONFIG(settings, learningRate,      "learningRate",         0.01);
    CONFIG(settings, downSamplingFactor,"downSamplingFactor",   4.0);

    if(bg)
    {
        delete bg;
    }
    bg = new cv::BackgroundSubtractorMOG2(history,varThreshold,isShadowDetection);


    bg->set("nmixtures",nmixtures);
    bg->set("backgroundRatio",backgroundRatio);
    bg->set("varThresholdGen",varThresholdGen);
    bg->set("fVarInit",fVarInit);
    bg->set("fCT",fCT);

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
         cv::Mat rawImageSmall;
         cv::Mat foregroundMaskSmall;
         cv::resize(rawImage, rawImageSmall,cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_AREA);
         cv::Mat foregroundMask;    // From frames (current)

         bg->operator()(rawImageSmall,foregroundMaskSmall,learningRate);
         cv::erode(foregroundMaskSmall,foregroundMaskSmall,cv::Mat(),cv::Point(-1,-1), erotions);
         cv::dilate(foregroundMaskSmall,foregroundMaskSmall,cv::Mat(),cv::Point(-1,-1), dilations);
         cv::threshold(foregroundMaskSmall,foregroundMaskSmall,230,255,CV_THRESH_BINARY);
         cv::resize(foregroundMaskSmall, foregroundMask, cv::Size(0,0), downSamplingFactor,downSamplingFactor, CV_INTER_NN);

         camera.addImage("foregroundMask", foregroundMask);
     }
 }

} // namespace image_processing
