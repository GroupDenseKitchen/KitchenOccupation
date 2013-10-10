#include "BackgroundModel.hpp"

BackgroundModel::BackgroundModel(){

}
BackgroundModel::~BackgroundModel(){

}

bool BackgroundModel::init()
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
    bg.setBool("bShadowDetection",isShadowDetection);
    bg.set("nmixtures",nmixtures);
    bg.set("backgroundRatio",backgroundRatio);

    //TODO: more parameters should be set, see above and clean it up, function should get inparameters...


    return true;
}

 void BackgroundModel::process(FrameList frames)
 {
     // TODO: Get rawImage matrix
     cv::Mat rawImage;      // From frames (current)
     cv::Mat foreground;    // From frames (current)

     bg(rawImage,foreground,0.001);

     cv::erode(foreground,foreground,cv::Mat(),cv::Point(-1,-1),erotions);
     cv::dilate(foreground,foreground,cv::Mat(),cv::Point(-1,-1),dilations);
     cv::threshold(foreground,foreground,230,255,CV_THRESH_BINARY);

     // TODO: Save foreground to matrix
 }
