#include "EntryExitCounter.hpp"


namespace image_processing
{
EntryExitCounter::EntryExitCounter() {}

EntryExitCounter::~EntryExitCounter() {}

bool EntryExitCounter::initialize(configuration::ConfigurationManager& settings)
{
    isInitialized = true;
    return isInitialized;
}

void EntryExitCounter::process(FrameList &frames)
{
    if(frames.hasPrevious())
    {
        for(unsigned int n = 0; n < frames.getCurrent().getCameras().size(); n++)
        {
            if(frames.hasDoorMask())
            {
                CameraObject  *cameraCurr = &frames.getCurrent().getCameras()[n];
                CameraObject  *cameraPrev = &frames.getPrevious().getCameras()[n];
                cameraCurr->setEntered(cameraPrev->getEntered()); //Get data from last frame
                cameraCurr->setExited(cameraPrev->getExited());   //Get data from last frame
                cv::Mat doorMask = frames.getDoorMask(); //Get the door mask

                for(std::vector<Object>::iterator object = cameraCurr->getTransitionaryObjects().begin(); object != cameraCurr->getTransitionaryObjects().end(); object++)
                {
                    cv::Point2d pos = object->exitPoint;
                    if(isInsidePolygon(doorMask, pos) && object->hasPassedMasksOne && object->hasPassedMasksTwo && object->hasPassedMasksThree)
                    {
                        cameraCurr->setExited(cameraCurr->getExited()+1);
                    }
                }
                cameraCurr->getTransitionaryObjects().clear();

                for(std::vector<Object>::iterator object = cameraCurr->getObjects().begin(); object != cameraCurr->getObjects().end(); object++)
                {
                    cv::Point2d entryPosition = object->entryPoint;
                    if(isInsidePolygon(doorMask, entryPosition) && object->hasPassedMasksOne && object->hasPassedMasksTwo && object->hasPassedMasksThree && !object->hasAlreadyEntered)
                    {
                        cameraCurr->setEntered(cameraCurr->getEntered()+1);
                        object->hasAlreadyEntered = true;
                    }
                }

                //Set population for a specific RoomID corresponding to the current camera.
                std::string currentRoomID = frames.getCurrent().getCameras()[n].getRoomID();
                int exitedThisFrame = cameraCurr->getExited()-cameraPrev->getExited();
                int enteredThisFrame =  cameraCurr->getEntered()-cameraPrev->getEntered();
                int prevPopulation = frames.getPrevious().getPopulationInRoomID(currentRoomID);
                frames.getCurrent().setPopulationInRoomID(prevPopulation+enteredThisFrame-exitedThisFrame, currentRoomID);


                //------------------ Debug writes nr of people that enters/exits into debugImage ------------------//
                if(!cameraCurr->hasImage("debugImage"))
                    cameraCurr->addImage("debugImage", cameraCurr->getImage("rawImage").clone());
                cv::Mat debugImage = cameraCurr->getImage("debugImage");
                std::string text = "";
                std::string text2 = "";
                int fontFace = cv::FONT_HERSHEY_PLAIN;
                double fontScale = 1;
                int thickness = 1;
                cv::Point2d pos1(10,20);
                cv::Point2d pos2(10,40);
                text = "Entered: " + std::to_string(cameraCurr->getEntered());
                putText(debugImage, text, pos1, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
                text2 = "Exited: " + std::to_string(cameraCurr->getExited());
                putText(debugImage, text2, pos2, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
                //------------------------------------------------------------------------------------------------//
            }
        }

        /* Sum all room populations into one. Since roomId's are always different from each other,
           totalPopulation is really a debug variable that now is just printed. Works only
           for one camera at the moment.*/
        totalPopulation = 0;
        for(unsigned int n = 0; n < frames.getCurrent().getCameras().size(); n++) {
            std::string currentRoomID = frames.getCurrent().getCameras()[n].getRoomID();
            totalPopulation = totalPopulation + frames.getCurrent().getPopulationInRoomID(currentRoomID);
        }
        //--------------------------------- Debug, writes population to debugImage --------------------------------//
        std::vector<CameraObject> cameras = frames.getCurrent().getCameras();
        if(cameras.size() > 0){
            //CameraObject  *cameraCurr = &frames.getCurrent().getCameras()[0];
            CameraObject  *cameraCurr = &cameras[0];
            std::string text = "";
            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 1;
            text = "Is inside: " + std::to_string(totalPopulation);
            cv::Point2d pos3(10,60);
            cv::Mat debugImage = cameraCurr->getImage("debugImage");
            putText(debugImage, text, pos3, fontFace, fontScale, cv::Scalar(0,255,0), 1, 8);
        }
        //--------------------------------------------------------------------------------------------------------//
    }
}

bool isInsidePolygon(cv::Mat mask, cv::Point2d point)
{
    if(point.x >= 0 && point.y >= 0)
    {
        return mask.at<cv::Vec3b>(point)[0] == 255;
    }
    return false;
}
}
