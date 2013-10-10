#ifndef DEBUG_H
#define DEBUG_H

#include "../DenseKitchen.hpp"

class DenseKitchen;
//TODO this class is supposed to be friends with everyone...


/*!
 *  \brief     Debugging utilities.
 */
namespace debugging
{
/*!
 *  \brief     Debuging module with introspection capabilities into all
 *             other classes in this project.
 *  \version   0.1
 *  \date      2013-10-07
 */
class Debug
{
public:

    /*!
       \brief   Constructor.
    */
    Debug();

    /*!
       \brief   Destructor.
    */
    ~Debug();

    /*!
       \brief   Start the debugging routine.
    */
    void start(DenseKitchen *state);

    /*!
       \brief   ???
    */
    void process();
};
}

#endif
