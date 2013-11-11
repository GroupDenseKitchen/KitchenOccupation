#include "EntryExitCounter.hpp"


namespace image_processing
{
EntryExitCounter::EntryExitCounter() {

}

EntryExitCounter::~EntryExitCounter() {

}

bool EntryExitCounter::initialize(configuration::ConfigurationManager& settings) {
    isInitialized = true;
    // check that the vector with masks is the size n-cameras.
    return isInitialized;
}

void EntryExitCounter::process(FrameList &frames)
{
    if(frames.hasPrevious())
    {

        for(int n = 0; n < frames.getCurrent().getCameras().size(); n++)
        {
            if(frames.hasDoorMask()){


                CameraObject  *cameraCurr = &frames.getCurrent().getCameras()[n];
                CameraObject  *cameraPrev = &frames.getPrevious().getCameras()[n];
                cameraCurr->setEntered(cameraPrev->getEntered()); //pass data from last frame
                cameraCurr->setExited(cameraPrev->getExited()); //pass data from last frame
                cv::Mat mask = frames.getDoorMask();

                for(std::vector<Object>::iterator object = cameraCurr->getTransitionaryObjects().begin(); object != cameraCurr->getTransitionaryObjects().end(); object++)
                {
                    cv::Point2d pos = object->exitPoint;
                    if(isInsidePolygon(mask, pos))
                    {
                        cameraCurr->setEntered(cameraCurr->getEntered()+1);
                    }
                }
                cameraCurr->getTransitionaryObjects().clear();

                for(std::vector<Object>::iterator object = cameraCurr->getNewlyFoundObjects().begin(); object != cameraCurr->getNewlyFoundObjects().end(); object++)
                {
                    cv::Point2d pos = object->entryPoint;
                    if(isInsidePolygon(mask, pos))
                    {
                        cameraCurr->setExited(cameraCurr->getExited()+1);
                    }
                }
                cameraCurr->getNewlyFoundObjects().clear();

                // Debug writes nr of people that enters/exits into raw image
                cv::Mat raw = cameraCurr->getImage("rawImage");
                std::string text = "";
                std::string text2 = "";
                int fontFace = cv::FONT_HERSHEY_PLAIN;
                double fontScale = 1;
                int thickness = 1;
                cv::Point2d pos1 = {10,15};
                cv::Point2d pos2 = {10,35};
                text = "Entered: " + std::to_string(cameraCurr->getEntered());
                putText(raw, text, pos1, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
                text2 = "Exited: " + std::to_string(cameraCurr->getExited());
                putText(raw, text2, pos2, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            }
        }
    }

}

bool EntryExitCounter::isInsidePolygon(cv::Mat mask, cv::Point2d point){
    if(point.x >= 0 && point.y >= 0){
        if(mask.at<cv::Vec3b>(point)[0] == 255){
            return true;
        }
    } else {
        return false;
    }
}

}
