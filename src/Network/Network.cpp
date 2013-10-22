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

bool Network::initialize(configuration::ConfigurationManager& settings)
{
    // Check if the necessary variables are available
    bool hasSettings = settings.hasBool("isTesting") &&
                       settings.hasInt("nCameras") &&
                       settings.hasString("videoFilePath");
    if(!hasSettings) {
        LOG("Network Error", "One ore more properties undefined");
        return false;
    }

    nCameras = settings.getInt("nCameras");
    isTesting = settings.getBool("isTesting");
    // If read from file
    if(isTesting) {
        std::string filePath = settings.getString("videoFilePath");
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
        for (unsigned int i = 0; i != streams.size(); i++) {
            CameraObject cam;
            cv::Mat rawImage;
            // Grab and retrieve image
            success = streams[i].read(rawImage);
            success = streams[i].read(rawImage);
            success = streams[i].read(rawImage);
            std::cerr << "Dis mah frame count: " << streams[i].get(CV_CAP_PROP_FRAME_COUNT) << std::endl;
            std::cerr << "Dis mah eff pee ess: " << streams[i].get(CV_CAP_PROP_FPS) << std::endl;
            std::cerr << "Dis mah frame index: " << streams[i].get(CV_CAP_PROP_POS_FRAMES) << std::endl;
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
        for (unsigned int i = 0; i != streams.size(); i++) {
            streams[i].grab();
        }
        // Retrieve all images
        for (unsigned int i = 0; i != streams.size(); i++) {
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
