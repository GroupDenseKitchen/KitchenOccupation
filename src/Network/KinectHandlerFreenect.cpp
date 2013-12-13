#include "KinectHandlerFreenect.hpp"

namespace kinect
{

KinectHandler::KinectHandler()
{
}

KinectHandler::~KinectHandler()
{
    std::cout << "Shutting down freenect" << std::endl;
}

bool KinectHandler::initialize()
{
    freenect_context *f_ctx;
    if (freenect_init(&f_ctx, NULL) < 0) {
            LOG("Kinect handler error","freenect_init() failed\n");
            return false;
        }

    nDevices = freenect_num_devices (f_ctx);
    if (nDevices == 0) {
        return false;
    } else {
        return true;
    }
}

KinectFrame* KinectHandler::readFrame(int deviceID)
{
    KinectFrame* frame = new KinectFrame;

    // ------------ Depth ---------------
    std::uint16_t *depthData = 0;
    if (freenect_sync_get_depth( (void**)&depthData, &frame->timestamp, deviceID, FREENECT_DEPTH_11BIT) ) {
        LOG( "KinectHandler","Error reading from kinnect. Is it connected?" );
        return NULL;
    }
    cv::Mat depthImage( cv::Size(640,480), CV_8UC1 );
    for (int i = 0; i < 640*480; ++i) {
        if(depthData[i] == 2047) {
            depthImage.data[i] = 0;
        } else {
            depthImage.data[i] = static_cast<std::uint8_t>(depthData[i]/4);
        }
    }
    //frame->depthImage = depthImage;
    cv::cvtColor(depthImage,frame->depthImage,CV_GRAY2BGR);

    // ------------- BGR -----------------
    std::uint8_t *rgbData = 0;
    std::uint32_t timestampRgb;
    if ( freenect_sync_get_video((void**)&rgbData, &timestampRgb, deviceID, FREENECT_VIDEO_RGB) ) {
        LOG( "KinectHandler","Error reading from kinnect. Is it connected?" );
        return NULL;
    }
    cv::Mat rgbImage = cv::Mat( cv::Size(640,480),CV_8UC3,rgbData ).clone();
    cv::cvtColor( rgbImage, frame->bgrImage, CV_RGB2BGR );

	return frame;
}

int KinectHandler::getnDevices()
{
	return nDevices;
}


} // namespace kinect
