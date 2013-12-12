#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/Frame.hpp"
#include "../Utilities/Timer.hpp"
#include "../Configuration/ConfigurationManager.hpp"

#ifdef __APPLE__
    #include "KinectHandlerFreenect.hpp"
#else
    #include "KinectHandlerOpenNi.hpp"
#endif // ifdef __APPLE__


/*!
 * \brief     the network namespace contains all system I/O functionality (sensors and web interface).
 */
namespace network
{
/*!
 * \brief       The Network class handles all system I/O.
 * \details     The Network class manages all system input/output, i.e. sampling the sensors and posting output data
 *              on the web server specified by the configuration file. Currently, support exists for all OpenCV-compatible cameras
 *              as well as the Microsoft Kinect depth sensor. The class is designed to be as modular as possible,
 *              allowing for a relatively easy integration of new sensor types into the system.
 *              The Network class also supports running several sensors in parallel.
 */
class Network
{
public:
    /*!
     * \brief   Constructor.
     */
	Network();

    /*!
     * \brief   Destructor.
     */
	~Network();

    /*!
     * \brief          Initializes the network module.
     * \details        The tries to initialize a Network object by opening up the necessary video streams specified by the
     *                 settings parameter. If this for some reason is not possible,
     *                 an error log message is created and the function retirns false.
     * \param settings A configuration object containing program settings.
     * \return         True if successful.
     */
    bool initialize(configuration::ConfigurationManager& settings);

    /*!
     * \brief   Destroys the video streams, thereby resetting the network module.
     */
    void reset();

    /*!
     * \brief dequeFrame Returns a pointer to the next frame.
     * \details          Tries to sample the different vidoe streams and returns a pointer to a newly created frame object if
     *                   all samplings are successful.
     * \return           Returns zero if no frame is available.
     */
    Frame* dequeFrame(void);

    /*!
     * \brief       Send data to the web server.
     * \details     The data that is presented on the web server is:
     *              - Number of people currently in side the room.
     *              - If there is a queue to enter the room or not.
     * \param frame Frame whose data is to be broadcasted
     */
    void broadcastData(Frame* frame);

private:
    bool initNetworkCameras(configuration::ConfigurationManager& settings,
                            std::vector<std::string> cameraPaths);
    bool loadVideoFiles(configuration::ConfigurationManager& settings,
                        std::vector<std::string> filePaths);


    Frame* getFileFrame();
    Frame* getNetworkCamFrame();
    Frame* getKinectFrame();

    kinect::KinectHandler kinects;

    bool firstFrame;        // Is this the first frame?
    bool runFromFile;       // True if reading from file
    bool useKinect;

    Timer timer;
    Frame nextFrame;
    std::vector<cv::VideoCapture> streams;

};

}
#endif
