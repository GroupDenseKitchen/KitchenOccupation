#ifndef ABSTRACTALGORITHM_H
#define ABSTRACTALGORITHM_H
#include "FrameList.hpp"

class AbstractAlgorithm
{
public:
    AbstractAlgorithm();

    virtual void process(FrameList frames) = 0;
};

#endif // ABSTRACTALGORITHM_H
