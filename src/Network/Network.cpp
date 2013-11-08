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
    bool hasSelector  = settings.hasBool("runFromFile");
    if (!hasSelector) {
        LOG("Network Error", "Boolean \"runFromFile\" undefined in config file.");
        return false;
    } else {
        runFromFile = settings.getBool("runFromFile");
    }

    bool hasFilePaths = settings.hasStringSeq("videoFilePaths");
    bool hasCamPaths  = settings.hasStringSeq("cameraPaths");

    bool hasValidSettings = (runFromFile && hasFilePaths) || (!runFromFile && hasCamPaths);

    if(!hasValidSettings) {
        LOG("Network Error", "std::vector<std::string> \"videoFilePaths\" or \"cameraPaths\" undefined in config file.");
        return false;
    }


    std::vector<std::string> filePaths, cameraPaths;
    filePaths = settings.getStringSeq("videoFilePaths");
    if (runFromFile) {
        filePaths = settings.getStringSeq("videoFilePaths");
    } else {
        cameraPaths = settings.getStringSeq("cameraPaths");
    }

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
        for (unsigned int i = 0; i < cameraPaths.size(); i++) {
            std::cerr << cameraPaths[i];
            cv::VideoCapture cam("http://root:pass@169.254.192.68/axis-cgi/mjpg/video.cgi?resolution=640x480&.mjpg");
            //cv::VideoCapture (cameraPaths[i]);
            if (cam.isOpened()) {
                streams.push_back(cam);
            } else {
            LOG("Network Warning", "Error opening camera stream at: " << cameraPaths[i]);
            }
        }
        if (streams.size() > 0) {
            settings.setInt("nCameras", (int)streams.size());
            return true;
        } else {
            return false;
        }
    }

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
        CameraObject cam;
        cv::Mat rawImage;
        success = false;
        do {

            success = streams[0].read(rawImage);


        } while (!success);
        cam.setRoomID(std::to_string(0));
        cam.addImage("rawImage", rawImage);
        frame->addCamera(cam);

/*
        CameraObject cam;
        cv::Mat rawImage;
        success = streams[0].read(rawImage);

        cam.setRoomID(std::to_string(0));
        cam.addImage("rawImage", rawImage);
        frame->addCamera(cam);
  */      /*
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
        */
    }

    return frame;
}


} // namespace network
