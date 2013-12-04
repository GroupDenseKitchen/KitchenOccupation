#ifndef KINECTHANDLERFREENECT_HPP
#define KINECTHANDLERFREENECT_HPP

#include <cstdint>

#include "../Utilities/utilities.hpp"
#include "../Configuration/ConfigurationManager.hpp"

#include <libfreenect.h>
#include <libfreenect_sync.h>

namespace kinect
{
struct KinectFrame 
{
    cv::Mat bgrImage;
	cv::Mat depthImage;
    std::uint32_t timestamp;
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
     * \brief readFrame 	Returns a pointer to a KinectFrame containing the last images.
     * \param deviceId		The index (zero based) of the device to be read. 
     * \return 				Returns zero if no frame is available.
     */
    KinectFrame* readFrame(int deviceID);
	
	/*!
	 * \brief getnDevices	The number of active devices
	 */
	int getnDevices();


private:
    int nDevices;


};

} // namspace kinect
#endif
