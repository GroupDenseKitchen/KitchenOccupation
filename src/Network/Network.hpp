#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include "../utilities/utilities.hpp"
#include "../utilities/Frame.hpp"

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

    /*!
       \brief   Start the network module.
    */
    void start();

    /*!
       \brief   Weather or not a new unprocessed frame is fetchable.
    */
    bool hasNewFrame();

    /*!
       \brief   Deque the next frame in cronological order.
    */
    Frame dequeFrame();
private:

};

}
#endif
