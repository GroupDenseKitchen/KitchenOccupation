#include "KinectHandlerOpenNi.hpp"

namespace kinect
{

KinectHandler::KinectHandler()
{
    depthWritePath = "depthStream.avi";
    colorWritePath = "colorStream.avi";
    networkWritePath = "networkStream.avi";
}

KinectHandler::~KinectHandler()
{
    std::cout << "Shutting down OpenNI" << std::endl;
    openni::OpenNI::shutdown();
}

bool KinectHandler::initialize(configuration::ConfigurationManager& settings)
{
    // ------------- Setup OpenNI ----------------------
    // Initiallize OpenNI
    openni::OpenNI::initialize();
    std::string openNIError = openni::OpenNI::getExtendedError();
    if(openNIError.empty()){
        std::cout << "Successfully initialized OpenNI" << std::endl;
    } else {
        std::cout << openNIError << std::endl;
    }

    // Look for devices
    openni::Array<openni::DeviceInfo>* devices = new openni::Array<openni::DeviceInfo>;
    openni::OpenNI::enumerateDevices(devices);
    nDevices = devices->getSize();

    std::cout << "Available devices: " << nDevices << std::endl;

    // If any interesting device exists
    if(nDevices > 0){

        const char* deviceURI = openni::ANY_DEVICE;

        // Try to open device
        rc = device.open(deviceURI);
        if (rc != openni::STATUS_OK)
        {
            printf("Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
            openni::OpenNI::shutdown();
            return false;
        }

        rc = depth.create(device, openni::SENSOR_DEPTH);
        if (rc == openni::STATUS_OK)
        {
            rc = depth.start();
            if (rc != openni::STATUS_OK)
            {
                printf("Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
                depth.destroy();
                return false;
            }
        }
        else
        {
            printf("Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
            return false;
        }

        rc = color.create(device, openni::SENSOR_COLOR);
        if (rc == openni::STATUS_OK)
        {
            rc = color.start();
            if (rc != openni::STATUS_OK)
            {
                printf("Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
                color.destroy();
                return false;
            }
        }
        else
        {
            printf("Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
            return false;
        }

        if (!depth.isValid() || !color.isValid())
        {
            printf("SimpleViewer: No valid streams. Exiting\n");
            openni::OpenNI::shutdown();
            return false;
        }
    }

    streams = new openni::VideoStream*[2];
    streams[0] = &depth;
    streams[1] = &color;

    return true;
}

KinectFrame* KinectHandler::readFrame(int deviceId)
{
    KinectFrame* kinectFrame = new KinectFrame;
    cv::Mat cvColorImage;
    cv::Mat cvDepthImage;

    // --------- Read from VideoStream -------------

    // ------------ Depth -------------
    depth.readFrame(&depthFrame);
    cvDepthImage = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(),
            CV_16U, (char*)depthFrame.getData());

    // The max value from the kinect sensor is somewhare around 5000
    // based on empirical experiments
    cvDepthImage.convertTo(cvDepthImage, CV_16U, 255*255/5000);
    cvDepthImage.convertTo(cvDepthImage, CV_8U, 1.0/255);
    cv::cvtColor(cvDepthImage, cvDepthImage, CV_GRAY2BGR);

    // ------------ Record Color -------------
    color.readFrame(&colorFrame);
    cvColorImage = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(),
            CV_8UC3, (char*)colorFrame.getData());

    cv::cvtColor(cvColorImage, cvColorImage, CV_RGB2BGR);

    kinectFrame->depthImage = cvDepthImage.clone();
    kinectFrame->bgrImage = cvColorImage.clone();

    return kinectFrame;
}

int KinectHandler::getnDevices()
{
	//TODO stub
	return nDevices;
}

} // namespace kinect
