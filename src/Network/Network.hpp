#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include "../utilities/utilities.hpp"
#include "../utilities/Frame.hpp"
#include "../Configuration/ConfigurationManager.hpp"

/*!
 *  \brief     Network contains all network functionality.
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

    /*
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

private:
    bool firstFrame;    // Is this the first frame?
    bool isTesting;     // True if reading from file

    int nCameras;       // Number of cameras

    Frame nextFrame;

    std::vector<cv::VideoCapture> streams;
};

}
#endif
