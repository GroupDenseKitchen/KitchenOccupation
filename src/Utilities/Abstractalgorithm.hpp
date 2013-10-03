#ifndef ABSTRACTALGORITHM_H
#define ABSTRACTALGORITHM_H
#include "FrameList.hpp"
/*! 
 *  \brief     Base class for pipeline algorithms
 *  \details   Derive form this class to maintain a simple interface for all algorithms used in the processing pipeline.
 *  \version   1.0
 */


class AbstractAlgorithm
{
public:
    virtual void process(FrameList frames) = 0;
};

#endif // ABSTRACTALGORITHM_H
