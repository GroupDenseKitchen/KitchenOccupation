#ifndef KINECTOPENNI_H
#define KINECTOPENNI_H

#include "../Utilities/utilities.hpp"

#include <OpenNI.h>
#undef min
#undef max

/*!
 * \brief   The kinect namespace contains functionality for reading rgb and depth images from a kinect 360 device.
 */
namespace kinect
{
/*!
 * \brief   Struct for handling output data created by the KinectHandler Class.
 */
struct KinectFrame 
{
    cv::Mat bgrImage;
	cv::Mat depthImage;
};

/*!
 * \brief   Handler for the Kinect sensors.
 * \details The class is a hardware interface for the Microsoft Kinect and contains functionality to convert
 *          output RGB and depth streams into OpenCV images.
 */
class KinectHandler
{
public:
    /*!
     * \brief   Constructor.
     */
	KinectHandler();

    /*!
     * \brief   Destructor.
     */
	~KinectHandler();

    /*!
     * \brief 	        Initializes the Kinect sensor.
     * \return          True if any devices were found.
     */
    bool initialize();
	
    /*!
     * \brief          Returns a pointer to the latest KinectFrame containing the last images.
     * \param deviceId The index (zero based) of the device to be read.
     * \return         Returns zero if no frame is available.
     */
    KinectFrame* readFrame(int deviceId);
	
	/*!
     * \brief   getnDevices
     * \return  The number of active devices.
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
