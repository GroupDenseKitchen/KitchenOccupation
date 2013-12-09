#include "KinectHandlerFreenect.hpp"

#include <QDebug>


namespace kinect
{

KinectHandler::KinectHandler()
{
    //TODO: Check if this needs to be expanded..
}

KinectHandler::~KinectHandler()
{
    std::cout << "Shutting down freenect" << std::endl;
}

bool KinectHandler::initialize(configuration::ConfigurationManager& settings)
{
    //Try to grab data once to make sure device is connected
    std::uint16_t *depthData = 0;
    std::uint32_t timestamp = 0;
    if (freenect_sync_get_depth( (void**)&depthData, &timestamp, 0, FREENECT_DEPTH_11BIT) ) {
        nDevices = 0;
        return false;
    } else {
        nDevices = 1;
        return true;
    }
}

KinectFrame* KinectHandler::readFrame(int deviceID)
{
    KinectFrame* frame = new KinectFrame;

    // ------------ Depth ---------------
    std::uint16_t *depthData = 0;
    if (freenect_sync_get_depth( (void**)&depthData, &frame->timestamp, 0, FREENECT_DEPTH_11BIT) ) {
        LOG( "KinectHandler","Error reading from kinnect. Is it connected?" );
        return NULL;
    }
    cv::Mat depthImage( cv::Size(640,480), CV_8UC1 );
    for (int i = 0; i < 640*480; ++i) {
        if(depthData[i] == 2047) {
            depthImage.data[i] = 0;
        } else {
            depthImage.data[i] = (std::uint8_t)(depthData[i]/8);
        }
    }
    //frame->depthImage = depthImage;
    cv::cvtColor(depthImage,frame->depthImage,CV_GRAY2BGR);

    // ------------- BGR -----------------
    std::uint8_t *rgbData = 0;
    std::uint32_t timestampRgb;
    if ( freenect_sync_get_video((void**)&rgbData, &timestampRgb, 0, FREENECT_VIDEO_RGB) ) {
        LOG( "KinectHandler","Error reading from kinnect. Is it connected?" );
        return NULL;
    }
    cv::Mat rgbImage = cv::Mat( cv::Size(640,480),CV_8UC3,rgbData ).clone();
    cv::cvtColor( rgbImage, frame->bgrImage, CV_RGB2BGR );

	return frame;
}

int KinectHandler::getnDevices()
{
	//TODO stub
	return nDevices;
}


} // namespace kinect
