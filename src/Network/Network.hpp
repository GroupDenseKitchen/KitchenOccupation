#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include "../utilities/utilities.hpp"
#include "../utilities/Frame.hpp"

/*!
 *  \brief     Network
 *  \details   Contains network functionality.
 */
namespace network
{
/*!
 *  \brief     Network module.
 *  \details   The interface to the network functionality.
 *  \version   0.1
 *  \date      2013-10-07
 */
class Network
{
public:
	Network();
	~Network();

    /*!
       \brief   Append the FrameList with the latest Frame.
    */
    void start();

    /*!
       \brief   Append the FrameList with the latest Frame.
    */
    bool hasNewFrame();

    /*!
       \brief   Append the FrameList with the latest Frame.
    */
    Frame dequeFrame();
private:

};

}
#endif
