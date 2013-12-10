#ifndef KINECTOPENNI_H
#define KINECTOPENNI_H

#include "../Utilities/utilities.hpp"
#include "../Configuration/ConfigurationManager.hpp"

#include <OpenNI.h>
#undef min
#undef max

/*!
 *      \brief kinect contains functionality for reading rgb and depth images from a kinect 360 devise.
 */
namespace kinect
{
struct KinectFrame 
{
    cv::Mat bgrImage;
	cv::Mat depthImage;
};

/*!
 *  \brief     Handler for the kinect sensors.
 *  \version   0.1
 *  \date      2013-10-07
 */
class KinectHandler
{
public:
    /*!
       \brief   Constructor.
    */
	KinectHandler();

    /*!
       \brief   Destructor.
    */
	~KinectHandler();

    /*!
     * \brief 	initialize	Initializes the Kinect sensor.
     * \param 	settings 	A configuration object containing program settings. (not sure if needed)
	 * \return 				True if any devices were found.
     */
    bool initialize(configuration::ConfigurationManager& settings);
	
    /*!
     * \brief readFrame 	Returns a pointer to the latest KinectFrame containing the last images.
     * \param deviceId		The index (zero based) of the device to be read. 
	 * \return 				Returns zero if no frame is available.
	 * \details				
     */
    KinectFrame* readFrame(int deviceId);
	
	/*!
	 * \brief getnDevices	The number of active devices
	 */
	int getnDevices();


private:
    int nDevices;

    openni::Device device;
    openni::VideoStream depth, color;
    openni::Status rc;
    openni::VideoStream** streams;

    openni::VideoFrameRef depthFrame, colorFrame;
    int changedIndex;

    std::string depthWritePath;
    std::string colorWritePath;
    std::string networkWritePath;
};

} // namspace kinect
#endif
