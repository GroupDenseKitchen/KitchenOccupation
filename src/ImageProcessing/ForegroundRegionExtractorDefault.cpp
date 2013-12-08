#include "ForegroundRegionExtractorDefault.hpp"

namespace image_processing
{
ForegroundRegionExtractorDefault::ForegroundRegionExtractorDefault() {

}

ForegroundRegionExtractorDefault::~ForegroundRegionExtractorDefault() {

}

bool ForegroundRegionExtractorDefault::initialize(configuration::ConfigurationManager &settings)
{
    isInitialized = true;

    CONFIG(settings, minimalArea, "minimalArea", 2500);

    return isInitialized;
}

void ForegroundRegionExtractorDefault::process(FrameList &frames)
{
    for(CameraObject & camera : frames.getCurrent().getCameras())
    {
        if(!camera.hasImage("foregroundMask"))
        {
            LOG("ImageProcessing Error", "Image \"foregroundMask\" not set in current frame!");
            return;
        }
        cv::Mat raw = camera.getImage("rawImage");

        foregroundMask = camera.getImage("foregroundMask");
        contours.clear();
        cv::findContours(foregroundMask.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        for(unsigned int c = 0; c < contours.size(); c++)
        {
            cv::Rect boundingBox = cv::boundingRect(contours[c]);
            if(boundingBox.height * boundingBox.width >= minimalArea)
            {
                cv::Moments mu = cv::moments(contours[c]);
                cv::Point2f centerOfMass = cv::Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
                cv::Rect boundingBox = cv::boundingRect(contours[c]);
                double area = cv::contourArea(contours[c]);

                camera.getObjects().push_back(Object(contours[c], boundingBox, centerOfMass, area));
                cv::rectangle(raw, camera.getObjects().back().boundingBox, cv::Scalar(0,0,255), 2);               // Debug
            }
        }
    }
}
}
