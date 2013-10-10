#include "Network.hpp"

namespace network
{

Network::Network()
{
    //TODO stub
}

Network::~Network()
{
    //TODO stub
}

bool Network::init(configuration::ConfigurationManager& settings)
{

    nCameras = settings.getNumberOfCameras();
    isTesting = settings.isTestingMode();
    // If read from file
    if(isTesting) {
        std::string filePath = settings.getVideoFilePath();
        cv::VideoCapture cam(filePath);
        if (cam.isOpened()) {
            streams.push_back(cam);
            return true;
        } else {
            LOG("Network Error", "Error reading video file " << filePath);
            return false;
        }
    } else {
        //TODO (read from network)
    }
    //if we reach this point, something went wrong
    return false;
}

Frame* Network::dequeFrame()
{

    Frame* frame = new Frame();
    bool success = true;

    if (isTesting) { // Loading from file
        for (int i = 0; i != streams.size(); i++) {
            CameraObject cam;
            cv::Mat rawImage;
            // Grab and retrieve image
            success = streams[i].grab() && streams[i].retrieve(rawImage);
            if (success)
            {
                cam.roomID = std::to_string(i);
                cam.addImage("rawImage", rawImage);
                frame->addCamera(cam);
            } else {
                LOG("Network Error", "Error retrieving frame from video file " << std::to_string(i) << ".");
                return 0;
            }
        }
    } else { // Using network cameras
        // Grab all images
        for (int i = 0; i != streams.size(); i++) {
            streams[i].grab();
        }
        // Retrieve all images
        for (int i = 0; i != streams.size(); i++) {
            CameraObject cam;
            cv::Mat rawImage;
            success = streams[i].retrieve(rawImage);
            if(success) {
                cam.roomID = std::to_string(i);
                cam.addImage("rawImage", rawImage);
                frame->addCamera(cam);
            } else {
                LOG("Network Error", "Error retrieving frame from camera " << std::to_string(i) << ".");
                return 0;
            }
        }
    }

    return frame;
}


} // namespace network
