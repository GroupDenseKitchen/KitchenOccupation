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
    REQUIRE(settings, cameraAmount,     "nCameras");

    if(isInitialized)
        initializeBackgroundModels(cameraAmount);

    return isInitialized;
}
void BackgroundModelMoG::process(FrameList &frames)
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

         cv::Mat rawImageSmall;
         cv::Mat foregroundMaskSmall;
         cv::resize(rawImage, rawImageSmall,cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_AREA);
         cv::Mat foregroundMask;    // From frames (current)

         backgroundModels[n]->operator()(rawImageSmall,foregroundMaskSmall,learningRate);
         cv::erode(foregroundMaskSmall,foregroundMaskSmall,cv::Mat(),cv::Point(-1,-1), erotions);
         cv::dilate(foregroundMaskSmall,foregroundMaskSmall,cv::Mat(),cv::Point(-1,-1), dilations);
         cv::threshold(foregroundMaskSmall,foregroundMaskSmall,230,255,CV_THRESH_BINARY);
         cv::resize(foregroundMaskSmall, foregroundMask, cv::Size(0,0), downSamplingFactor,downSamplingFactor, CV_INTER_NN);

         camera.addImage("foregroundMask", foregroundMask);
         n++;
     }
 }

 void BackgroundModelMoG::initializeBackgroundModels(int cameraAmount) {
     if(backgroundModels.size() > 0) {
         std::vector<cv::BackgroundSubtractorMOG2*>::iterator background = backgroundModels.begin();
         while(background != backgroundModels.end()) {
             backgroundModels.erase(background);
         }
     }
     for(int n = 0; n < cameraAmount; n++) {
         backgroundModels.push_back(new cv::BackgroundSubtractorMOG2(history,varThreshold,isShadowDetection));

         backgroundModels[n]->set("nmixtures",nmixtures);
         backgroundModels[n]->set("backgroundRatio",backgroundRatio);
         backgroundModels[n]->set("varThresholdGen",varThresholdGen);
         backgroundModels[n]->set("fVarInit",fVarInit);
         backgroundModels[n]->set("fCT",fCT);
     }
 }

} // namespace image_processing
