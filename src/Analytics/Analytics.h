#ifndef STATISTICS_MODULE_H
#define STATISTICS_MODULE_H

#include "../Utilities/include.h"

namespace statistics{
    class Analytics{
    public:
        Analytics();
        ~Analytics();
        void processFrame(FrameList frames);
    };
}

#endif