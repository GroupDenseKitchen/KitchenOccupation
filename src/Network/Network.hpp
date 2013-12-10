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

#include <QtNetwork/QtNetwork>
#include <QByteArray>

/*!
 *  \brief     Network contains all network and camera i/o functionality.
 */
namespace network
{
/*!
 *  \brief     The Network is the interface to the network functionality.
 *  \version   0.1
 *  \date      2013-10-07
 */
class Network
{
public:
    /*!
       \brief   Constructor.
    */
	Network();

    /*!
       \brief   Destructor.
    */
	~Network();

    /*!
     * \brief Initializes the network module.
     * \param settings A configuration object containing program settings
     */
    bool initialize(configuration::ConfigurationManager& settings);

    void reset();

    /*!
     * \brief dequeFrame returns a pointer to the latest frame.
     * \return Returns zero if no frame is available.
     */
    Frame* dequeFrame(void);

    /*!
     * \brief broadcastData Send data to the web server.
     * \param frame         Frame whose data is to be broadcasted
     * \details             The data that is presented on the web server is:
     *                      - Number of people in/out in current frame
     *                      - If there is a queue to enter the room or not.
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
    QUrl serverUrl;

    QNetworkAccessManager* nwam;
};

}
#endif
