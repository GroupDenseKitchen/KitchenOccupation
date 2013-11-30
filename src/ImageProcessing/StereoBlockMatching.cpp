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
    //CONFIG(settings, nmixtures,         "nmixtures",            5);
    REQUIRE(settings, cameraAmount,     "nCameras");

    //if(isInitialized)
        //initializeBackgroundModels(cameraAmount);

    return isInitialized;
}
void StereoBlockMatching::process(FrameList &frames)
{
    int n = 0;

    if(frames.getCurrent().getCameras().size()%2 == 0){
        for(int i = 0; i < frames.getCurrent().getCameras().size(); i = i+2)//CameraObject & camera : frames.getCurrent().getCameras())
        {
            // TODO: Check in init that size of the vector is an even number > 2
            CameraObject &leftCam = frames.getCurrent().getCameras()[i];
            CameraObject &rightCam = frames.getCurrent().getCameras()[i+1];
            if(!leftCam.hasImage("rawImage") || !rightCam.hasImage("rawImage"))
            {
                LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
                return;
            }

            cv::Mat rawImageLeft = leftCam.getImage("rawImage");
            cv::Mat rawImageRight = rightCam.getImage("rawImage");

            cv::Mat stereoDepthMap;

            cv::StereoSGBM blockMatcher;
            blockMatcher.SADWindowSize = 5;
            blockMatcher.numberOfDisparities = 192;
            blockMatcher.preFilterCap = 4;
            blockMatcher.minDisparity = -64;
            blockMatcher.uniquenessRatio = 1;
            blockMatcher.speckleWindowSize = 150;
            blockMatcher.speckleRange = 2;
            blockMatcher.disp12MaxDiff = 10;
            blockMatcher.fullDP = false;
            blockMatcher.P1 = 600;
            blockMatcher.P2 = 2400;

            cv::flip(rawImageLeft,rawImageLeft,1);

            blockMatcher(rawImageLeft, rawImageRight, stereoDepthMap);

            stereoDepthMap.convertTo(stereoDepthMap, CV_8UC1);

            leftCam.addImage("stereoDepthMap", stereoDepthMap);
            rightCam.addImage("stereoDepthMap", stereoDepthMap);
            n++;
        }
    } else {
        LOG("ImageProcessing Error", "Uneven number of cameras can't be block matched!");
    }
}
} // namespace image_processing
