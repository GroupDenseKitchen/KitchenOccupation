#include "Network.hpp"

namespace network
{

Network::Network() {}

Network::~Network() {}

bool Network::initialize(configuration::ConfigurationManager& settings)
{

    firstFrame = true;

    // Check if the necessary variables are available
    if (!settings.hasBool("runFromFile")) {
	printf("run from file undefined in config \n");
        LOG("Network Error", "Boolean \"runFromFile\" undefined in config file.");
        return false;
    } else {
        runFromFile = settings.getBool("runFromFile");
    }

    if (!settings.hasBool("useKinect")) {
	printf("use kinect undefined in config file \n");
        LOG("Network Error", "Boolean \"useKinect\" undefined in config file.");
        return false;
    } else {
        useKinect = settings.getBool("useKinect");
    }

    if (!runFromFile && useKinect) {
        return kinects.initialize();
    }

    bool hasFilePaths = settings.hasStringSeq("videoFilePaths");
    bool hasCamPaths  = settings.hasStringSeq("cameraPaths");

    bool hasValidSettings = (runFromFile && hasFilePaths) || (!runFromFile && hasCamPaths);

    if(!hasValidSettings) {
	printf("settings invalid! \n");
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
        return loadVideoFiles(settings, filePaths);
    } else {
        return initNetworkCameras(settings, cameraPaths);
    }
}

bool Network::initNetworkCameras(configuration::ConfigurationManager& settings,
                                 std::vector<std::string> cameraPaths)
{
    for (unsigned int i = 0; i < cameraPaths.size(); i++) {
        cv::VideoCapture cam(cameraPaths[i]);
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
        LOG("Network Error", "No camera streams from file(s)!");
        return false;
    }
}

bool Network::loadVideoFiles(configuration::ConfigurationManager& settings,
                             std::vector<std::string> filePaths)
{
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
        LOG("Network Error", "No camera streams from network!");
        return false;
    }
}


void Network::reset()
{
    streams.clear();
}

Frame* Network::dequeFrame()
{

    if (runFromFile) { // Loading from file
        return getFileFrame();
    } else if (useKinect) {
        return getKinectFrame();
    } else {
        return getNetworkCamFrame();
    }
}

void Network::broadcastData(Frame *frame)
{
    //TODO: stub
}


Frame *Network::getFileFrame()
{
    Frame* frame = new Frame();
    bool success = true;

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
            frame->setMomentaryFps(streams[i].get(CV_CAP_PROP_FPS));
        } else {
            LOG("Network Error", "Error retrieving frame from video file " << std::to_string(i) << ".");
            delete frame;
            return 0;
        }
    }
    frame->initRoomPopulations(frame->getCameras());
    return frame;
}

Frame *Network::getNetworkCamFrame()
{
    Frame* frame = new Frame();
    bool success = true;

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
            if (firstFrame) {
                frame->setMomentaryFps(0);
                firstFrame = false;
            } else {
                frame->setMomentaryFps(1.0/timer.getSeconds());
            }

        } else {
            LOG("Network Error", "Error retrieving frame from camera " << std::to_string(i) << ".");
            delete frame;
            timer.reset();
            return 0;
        }
    }
    timer.reset();
    frame->initRoomPopulations(frame->getCameras());
    return frame;
}

Frame *Network::getKinectFrame()
{
    Frame* frame = new Frame();
    kinect::KinectFrame* kinectFrame = 0;
    bool success = true;

    for (int i = 0; i < kinects.getnDevices(); i++) {
        CameraObject cam;
        kinectFrame = kinects.readFrame(i);
        if (kinectFrame) {
            cam.addImage("rawImage", kinectFrame->depthImage);
            cam.addImage("rawColorImage", kinectFrame->bgrImage.clone());
            cam.addImage("debugImage", kinectFrame->bgrImage);
            frame->addCamera(cam);
            if (firstFrame) {
                frame->setMomentaryFps(0);
                firstFrame = false;
            } else {
                frame->setMomentaryFps(1.0/timer.getSeconds());
            }
            delete kinectFrame;
        } else {
            LOG("Network Error", "Error retrieving frame from Kinect device number: " << std::to_string(i) << ".");
            delete frame;
            timer.reset();
            return 0;
        }
    }
    timer.reset();
    return frame;
}


} // namespace network
