#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include "../utilities/utilities.hpp"
#include "../utilities/Frame.hpp"
#include "../Configuration/ConfigurationManager.hpp"

namespace network{

class Network{
public:
	Network();
	~Network();

    /*!
     * \brief Initializes the network module.
     * \param settings A configuration object containing program settings
     */
    bool init(configuration::ConfigurationManager& settings);

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
