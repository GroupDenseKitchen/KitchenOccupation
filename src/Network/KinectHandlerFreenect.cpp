#include "KinectHandlerFreenect.hpp"

namespace kinect
{

KinectHandler::KinectHandler()
{
    //TODO stub
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
	//TODO stub
	KinectFrame* frame = 0;
	return frame;
}

int KinectHandler::getnDevices()
{
	//TODO stub
	return nDevices;
}

} // namespace kinect
