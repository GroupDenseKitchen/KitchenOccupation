#include "StereoBlockMatching.hpp"

namespace image_processing
{

StereoBlockMatching::StereoBlockMatching(){

}
StereoBlockMatching::~StereoBlockMatching(){

}

bool StereoBlockMatching::initialize(configuration::ConfigurationManager &settings)
{
    isInitialized = true;

    // Initialize variables
    //
    //               |VARIABLE          |NAME                  |DEFAULT
    //---------------------------------------------------------------
    CONFIG(settings, nmixtures,         "nmixtures",            5);
    CONFIG(settings, backgroundRatio,   "backgroundRatio",      0.95);
    CONFIG(settings, varThresholdGen,   "varThresholdGen",      16);
    CONFIG(settings, varThreshold,      "varThreshold",          16);
    CONFIG(settings, fVarInit,          "fVarInit",             15);
    CONFIG(settings, fCT,               "fCT",                  0.05);
    CONFIG(settings, isShadowDetection, "isShadowDetection",    true);
    CONFIG(settings, erotions,          "erotions",             3);
    CONFIG(settings, dilations,         "dilations",            3);
    CONFIG(settings, history,           "history",              500);
    CONFIG(settings, learningRate,      "learningRate",         0.0005);
    CONFIG(settings, downSamplingFactor,"downSamplingFactor",   3.0);
    REQUIRE(settings, cameraAmount,     "nCameras");

    if(isInitialized)
        initializeBackgroundModels(cameraAmount);

    return isInitialized;
}
void StereoBlockMatching::process(FrameList &frames)
{
    int n = 0;
    CameraObject* leftCam, rightCam;
    for(int i = 0; i < frames.getCurrent().getCameras().size(); i = i+2);//CameraObject & camera : frames.getCurrent().getCameras())
    {
        // TODO: Check in init that size of the vector is an even number > 2
        leftCam = &frames.getCurrent().getCameras()[i];
        rightCam = &frames.getCurrent().getCameras()[i+1];
        if(!leftCam->hasImage("rawImage") || rightCam->hasImage("rawImage"))
        {
            LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
            return;
        }

        cv::Mat rawImageLeft = leftCam->getImage("rawImage");
        cv::Mat rawImageRight = rightCam->getImage("rawImage");

        cv::Mat stereoDepthMap;

        cv::StereoSGBM blockMatcher;
        camera.addImage("stereoDepthMap", foregroundMask);
        n++;
    }
}


} // namespace image_processing
