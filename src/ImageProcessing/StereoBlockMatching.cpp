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

            cv::Mat leftGray;
            cv::Mat rightGray;

            cv::cvtColor(rawImageLeft, leftGray, CV_RGB2GRAY);
            cv::cvtColor(rawImageRight, rightGray, CV_RGB2GRAY);

            bool SGBM = false;
            bool BM = true;

            if(SGBM){
                // ------ Stereo SGBM ----------------------------
                cv::StereoSGBM stereoSGBM;
                stereoSGBM.minDisparity = -64;
                stereoSGBM.numberOfDisparities = 192;
                stereoSGBM.SADWindowSize = 5;
                stereoSGBM.preFilterCap = 4;
                stereoSGBM.uniquenessRatio = 1;
                stereoSGBM.P1 = 600;
                stereoSGBM.P2 = 2400;
                stereoSGBM.speckleWindowSize = 150;
                stereoSGBM.speckleRange = 2;
                stereoSGBM.disp12MaxDiff = 10;
                stereoSGBM.fullDP = false;

                cv::Mat stereoDepthSGBM;
                stereoSGBM(leftGray, rightGray, stereoDepthSGBM);
                normalize(stereoDepthSGBM, stereoDepthSGBM, 0, 255, CV_MINMAX, CV_8U);
                leftCam.addImage("StereoSGBM", stereoDepthSGBM);

            }
            if(BM){
                // ------ Stereo SG ------------------------------

                cv::StereoBM stereoBM;

                stereoBM.state->minDisparity = -64;
                stereoBM.state->numberOfDisparities = 192;
                stereoBM.state->SADWindowSize = 5;
                stereoBM.state->preFilterSize = 5;
                stereoBM.state->preFilterCap = 61;
                stereoBM.state->textureThreshold = 507;
                stereoBM.state->uniquenessRatio = 0;
                stereoBM.state->speckleWindowSize = 0;
                stereoBM.state->speckleRange = 8;
                stereoBM.state->disp12MaxDiff = 1;

/*
                int preset = CV_STEREO_BM_BASIC;
                int nDisparities = 128;
                int SADWindowSize = 5;
                stereoBM.init(preset, nDisparities, SADWindowSize);
                */


                cv::Mat stereoDepthBM;
                stereoBM(leftGray, rightGray, stereoDepthBM);
                normalize(stereoDepthBM, stereoDepthBM, 0, 255, CV_MINMAX, CV_8U);
                leftCam.addImage("StereoBM", stereoDepthBM);
/*
                cv::Mat diffImage;
                diffImage = leftGray - rightGray;
                leftCam.addImage("Diff Image", diffImage);

                cv::Mat gaussianBlured;
                cv::blur(stereoDepthMap, gaussianBlured, cv::Size(5,5));
                leftCam.addImage("Gaussian Blured", gaussianBlured);

                cv::Mat medianBlured;
                cv::medianBlur(stereoDepthMap, medianBlured, 21);
                leftCam.addImage("Median Blured", medianBlured);

                cv::Mat bilateralFiltered;
                int diameter = 10;
                double sigmaColor = 150;
                double sigmaSpace = 150;
                cv::bilateralFilter(stereoDepthMap, bilateralFiltered, diameter, sigmaColor, sigmaSpace);
                leftCam.addImage("Bilateral Filtered", bilateralFiltered);
                */
            }

            n++;
        }
    } else {
        LOG("ImageProcessing Error", "Uneven number of cameras can't be block matched!");
    }
}
} // namespace image_processing
