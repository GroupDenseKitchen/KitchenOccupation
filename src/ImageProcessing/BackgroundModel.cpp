#include "BackgroundModel.hpp"

BackgroundModel::BackgroundModel(){

}
BackgroundModel::~BackgroundModel(){

}

bool BackgroundModel::initialize(configuration::ConfigurationManager &configuration)
{
    //STANDARD VALUES
    nmixtures = 5;
    backgroundRatio = 0.9;
    varThresholdGen = 9;
    fVarInit = 15;
    //fVarMin = ;
    //fVarMax = ;
    fCT = 0.05;
    //fTau = ;
    isShadowDetection = false;
    erotions = 3;
    dilations = 3;
    history = 500;

    bg.set("history",history);
    bg.set("varThreshold",varThresholdGen);
    //bg.setBool("shadowDetection",isShadowDetection);
    bg.set("nmixtures",nmixtures);
    bg.set("backgroundRatio",backgroundRatio);

    //TODO: more parameters should be set, see above and clean it up, function should get inparameters...


    return true;
}

 void BackgroundModel::process(FrameList &frames)
 {
     //TODO: loop over all cameras...
     CameraObject camera = frames.getCurrent().getCameras().back();

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

     camera.addImage("foregroundMask", foregroundMask);
 }
