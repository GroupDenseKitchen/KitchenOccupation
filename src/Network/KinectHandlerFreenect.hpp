#ifndef KINECTHANDLERFREENECT_HPP
#define KINECTHANDLERFREENECT_HPP

#include <cstdint>

#include "../Utilities/utilities.hpp"
#include "../Utilities/Timer.hpp"

#include <libfreenect.h>
#include <libfreenect_sync.h>

#include <unistd.h>

/*!
 * \brief   The kinect namespace contains functionality for reading rgb and depth images from a kinect 360 device.
 */
namespace kinect
{
struct KinectFrame 
{
    cv::Mat bgrImage;
	cv::Mat depthImage;
    std::uint32_t timestamp;
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
       \brief   Constructor.
    */
	KinectHandler();

    /*!
       \brief   Destructor.
    */
	~KinectHandler();

    /*!
     * \brief 	        Initializes the Kinect sensor.
     * \return          True if any devices were found.
     */
    bool initialize();
	
    /*!
     * \brief readFrame 	Returns a pointer to a KinectFrame containing the last images.
     * \param deviceId		The index (zero based) of the device to be read. 
     * \return 				Returns zero if no frame is available.
     */
    KinectFrame* readFrame(int deviceID);
	
	/*!
     * \brief   getnDevices
     * \return  The number of active devices.
	 */
	int getnDevices();


private:
    int nDevices;
    Timer samplingTimer;


};

} // namspace kinect
#endif
