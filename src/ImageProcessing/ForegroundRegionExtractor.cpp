#include "ForegroundRegionExtractor.hpp"

namespace image_processing
{
    ForegroundRegionExtractor::ForegroundRegionExtractor() {

    }

    ForegroundRegionExtractor::~ForegroundRegionExtractor() {

    }

    bool ForegroundRegionExtractor::initialize(configuration::ConfigurationManager &configuration)
    {
        return true;
    }

    void ForegroundRegionExtractor::process(FrameList &frames) {
        //TODO: loop over all cameras...
        CameraObject & camera = frames.getCurrent().getCameras().back();

        if(!camera.hasImage("foregroundMask"))
        {
            LOG("ImageProcessing Error", "Image \"foregroundMask\" not set in current frame!");
            return;
        }

        foregroundMask = camera.getImage("foregroundMask");
        contours.clear();
        // TODO: is clone() necessary!?
        cv::findContours(foregroundMask.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        for(unsigned int c = 0; c < contours.size(); c++)
        {
            camera.objects.push_back(Object());
            camera.objects.back().boundingBox = cv::boundingRect(contours[c]);
        }
    }
}
