#include "KinectHandlerFreenect.hpp"


namespace kinect
{

KinectHandler::KinectHandler()
{
    //TODO: Check if this needs to be expanded..
    nDevices = 1;
}

KinectHandler::~KinectHandler()
{
    //TODO stub
}

bool KinectHandler::initialize(configuration::ConfigurationManager& settings)
{
	//TODO stub
	return false;
}

KinectFrame* KinectHandler::readFrame(int deviceID)
{
    KinectFrame* frame = new KinectFrame;
    //Depth
    std::uint16_t *depthData = 0;
    if (freenect_sync_get_depth( (void**)&depthData, &frame->timestamp, 0, FREENECT_DEPTH_11BIT) ) {
        LOG( "KinectHandler","Error reading from kinnect. Is it connected?" );
    }
    frame->depthImage = cv::Mat( cv::Size(640,480), CV_16UC1, depthData ).clone();

    //BGR
    std::uint8_t *rgbData = 0;
    std::uint32_t timestampRgb;
    if ( freenect_sync_get_video((void**)&rgbData, &timestampRgb, 0, FREENECT_VIDEO_RGB) ) {
        LOG( "KinectHandler","Error reading from kinnect. Is it connected?" );
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
