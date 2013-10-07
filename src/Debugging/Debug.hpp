#ifndef DEBUG_H
#define DEBUG_H

#include "../DenseKitchen.hpp"

class DenseKitchen;
//TODO this class is supposed to be friends with everyone...


/*!
 *  \brief     Debugging
 *  \details   Contains debugging utilities.
 */
namespace debugging
{
/*!
 *  \brief
 *  \details
 *  \version   0.1
 *  \date      2013-10-07
 */
class Debug
{
public:
    Debug();
    ~Debug();
    void start(DenseKitchen *state);
    void process();
};
}

#endif
