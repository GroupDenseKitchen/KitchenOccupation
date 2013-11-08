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
    bool hasSettings = settings.hasBool("runFromFile") &&
                       settings.hasInt("nCameras") &&
                       settings.hasStringSeq("videoFilePaths");
    if(!hasSettings) {
        LOG("Network Error", "One ore more properties undefined");
        return false;
    }

    nCameras = settings.getInt("nCameras");
    runFromFile = settings.getBool("runFromFile");
    std::vector<std::string> filePaths = settings.getStringSeq("videoFilePaths");

    if (runFromFile) {
        for (unsigned int i = 0; i < filePaths.size(); i++) {
            cv::VideoCapture cam(filePaths[i]);
            if (cam.isOpened()) {
                streams.push_back(cam);
            } else {
                LOG("Network Warning", "Error reading video file: " << filePaths[i]);
            }
        }
        if (streams.size() > 0) {
            settings.setInt("nCameras", (int)streams.size());
            return true;
        } else {
            return false;
        }

    } else {
        //TODO (read from network)
    }
/*
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
*/
    //if we reach this point, something went wrong
    return false;
}

void Network::reset()
{
    streams.clear();
}

Frame* Network::dequeFrame()
{

    Frame* frame = new Frame();
    bool success = true;

    if (runFromFile) { // Loading from file
        for (unsigned int i = 0; i != streams.size(); i++) {
            CameraObject cam;
            cv::Mat rawImage;
            // Grab and retrieve image
            success = streams[i].grab() && streams[i].retrieve(rawImage);
            if (success)
            {
                cam.setRoomID(std::to_string(i));
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
                cam.setRoomID(std::to_string(i));
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
