#include "Network.hpp"

namespace network
{

Network::Network() {}

Network::~Network()
{
#ifdef USECURL
    if (curlInitialized) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
#endif
}

bool Network::initialize(configuration::ConfigurationManager& settings)
{
    framesSinceLastPush = 0;
#ifdef USECURL
    if (!settings.hasString("webServerUrl")) {
        LOG("Network Warning", "no serverUrl specified");
        curlInitialized = false;
    } else {
        curlInitialized = true;

        curl_global_init(CURL_GLOBAL_ALL);

      curl = curl_easy_init();
      if(curl) {

            struct curl_slist *headers = NULL;

            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers );

            //curl_easy_setopt(curl, CURLOPT_VERBOSE, CURLINFO_HEADER_OUT);

            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt(curl, CURLOPT_URL, settings.getString("webServerUrl").c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      } else {
          curlInitialized = false;
          curl_easy_cleanup(curl);
          curl_global_cleanup();
          LOG("Network Warning", "Could not initialize curl, network communication disabled");
      }

    }
#endif //CURL

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
        return initOpenCVCameras(settings, cameraPaths);
    }
}

bool Network::initOpenCVCameras(configuration::ConfigurationManager& settings,
                                 std::vector<std::string> cameraPaths)
{
    for (unsigned int i = 0; i < cameraPaths.size(); i++) {
        cv::VideoCapture cam;
        if (cameraPaths[i].length() > 11) {
            cam.open(cameraPaths[i]);
        } else {
            cam.open(atoi(cameraPaths[i].c_str()));
        }
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

void Network::broadcastData(Frame& frame)
{
    
#ifdef USECURL
    if (curlInitialized) {
        char data[1337];
        int queStatus = frame.getQueStatus();
        int totalPopulation = 0;
        for(unsigned int n = 0; n < frame.getCameras().size(); n++) {
            std::string currentRoomID = frame.getCameras()[n].getRoomID();
            totalPopulation = totalPopulation + frame.getPopulationInRoomID(currentRoomID);
        }
        sprintf(data, "{\"queStatus\" : \"%d\", \"numOccupants\" : \"%d\"}", queStatus, totalPopulation);

        //check if the state changed, if it did not, don't bother sending any data
        if(strcmp(lastBroadcastState,data) == 0){
            return;
        }else{
            //remember the data we sent
            strcpy(lastBroadcastState,data);
        
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

                /* Perform the request, res will get the return code */
                res = curl_easy_perform(curl);
            }
        }
    }
#else
    //LOG("Network Message", "Due to conflicts with the Qt core library, communication with the server is impossible with the GUI enabled.")
#endif
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
            cam.addImage("debugImage",rawImage.clone());
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
            cam.setRoomID(std::to_string(i));
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
    frame->initRoomPopulations(frame->getCameras());
    return frame;
}


} // namespace network
