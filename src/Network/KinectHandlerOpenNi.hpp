#ifndef KINECTOPENNI_H
#define KINECTOPENNI_H

#include "../Utilities/utilities.hpp"
#include "../Configuration/ConfigurationManager.hpp"

#include <OpenNI.h>
#undef min
#undef max

/*!
 * \brief   kinect contains functionality for reading rgb and depth images from a kinect 360 devise.
 * \details TODO
 */
namespace kinect
{
/*!
 * \brief   TODO
 * \details TODO
 */
struct KinectFrame 
{
    cv::Mat bgrImage;
	cv::Mat depthImage;
};

/*!
 * \brief   Handler for the kinect sensors.
 * \details TODO
 * \version 0.1
 * \date    2013-10-07
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
     * \details        TODO
     * \param settings A configuration object containing program settings. (not sure if needed)
     * \return 		True if any devices were found.
     */
    bool initialize(configuration::ConfigurationManager& settings);
	
    /*!
     * \brief          Returns a pointer to the latest KinectFrame containing the last images.
     * \details        TODO
     * \param deviceId The index (zero based) of the device to be read.
     * \return         Returns zero if no frame is available.
     */
    KinectFrame* readFrame(int deviceId);
	
	/*!
     * \brief   The number of active devices.
     * \details TODO
     * \return  TODO
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
