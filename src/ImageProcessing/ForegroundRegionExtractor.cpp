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
        CameraObject &camera = frames.getCurrent().getCameras().back();

        if(!camera.hasImage("foregroundMask"))
        {
            LOG("ImageProcessing Error", "Image \"foregroundMask\" not set in current frame!");
            return;
        }

        cv::namedWindow("boundingboxes");           // Debug
        cv::Mat raw = camera.getImage("rawImage");  // Debug


        foregroundMask = camera.getImage("foregroundMask");
        contours.clear();
        // TODO: is clone() necessary!?
        cv::findContours(foregroundMask.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        for(unsigned int c = 0; c < contours.size(); c++)
        {
            cv::Rect rectangle = cv::boundingRect(contours[c]);
            if(rectangle.height * rectangle.width >= 2000)
            {
                camera.objects.push_back(Object());
                camera.objects.back().boundingBox = rectangle;
                cv::rectangle(raw, camera.objects.back().boundingBox, cv::Scalar(0,0,255), 2);     // Debug
            }
        }
        cv::imshow("boundingboxes", raw);     // Debug


    }
}
