#include "EntryExitCounter.hpp"


namespace image_processing
{
EntryExitCounter::EntryExitCounter() {

}

EntryExitCounter::~EntryExitCounter() {

}

bool EntryExitCounter::initialize(configuration::ConfigurationManager& settings) {
    isInitialized = true;

    CONFIG(settings, yMax, "yMax", 480/2);
    CONFIG(settings, yMin, "yMin", 480/5);
    CONFIG(settings, xMax, "xMax", 640);
    CONFIG(settings, xMin, "xMin", 0);

    return isInitialized;
}

void EntryExitCounter::process(FrameList &frames)
{
    if(frames.hasPrevious())
    {

        for(int n = 0; n < frames.getCurrent().getCameras().size(); n++)
        {
            CameraObject  *cameraCurr = &frames.getCurrent().getCameras()[n];
            CameraObject  *cameraPrev = &frames.getPrevious().getCameras()[n];
            cameraCurr->setEntered(cameraPrev->getEntered()); //pass data from last frame
            cameraCurr->setExited(cameraPrev->getExited());

            for(std::vector<Object>::iterator object = cameraCurr->getTransitionaryObjects().begin(); object != cameraCurr->getTransitionaryObjects().end(); object++)
            {
                cv::Point2d pos = object->exitPoint;
                if(pos.x<xMax && pos.x >xMin && pos.y < yMax && pos.y > yMin) //the intervals here needs to be camera specific
                {
                    cameraCurr->setEntered(cameraCurr->getEntered()+1);             
                }
            }
            cameraCurr->getTransitionaryObjects().clear();

            for(std::vector<Object>::iterator object = cameraCurr->getNewlyFoundObjects().begin(); object != cameraCurr->getNewlyFoundObjects().end(); object++)
            {
                cv::Point2d pos = object->entryPoint;
                if(pos.x<xMax && pos.x >xMin && pos.y < yMax && pos.y > yMin) //the intervals here needs to be camera specific
                {
                    cameraCurr->setExited(cameraCurr->getExited()+1);
                }
            }
            cameraCurr->getNewlyFoundObjects().clear();

            // Debug
            cv::Mat raw = cameraCurr->getImage("rawImage");
            std::string text = "";
            std::string text2 = "";
            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 1;
            int thickness = 1;
            cv::Point2d pos1 = {10,20};
            cv::Point2d pos2 = {10,35};
            text = "People Entered: " + std::to_string(cameraCurr->getEntered());
            putText(raw, text, pos1, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            text2 = "People Exited: " + std::to_string(cameraCurr->getExited());

            putText(raw, text2, pos2, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
        }
    }


}

}
