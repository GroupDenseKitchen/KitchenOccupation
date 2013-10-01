#ifndef STATISTICS_MODULE_H
#define STATISTICS_MODULE_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"

namespace statistics{
    class Analytics{
    public:
        Analytics();
        ~Analytics();
        void processFrame(FrameList frames);
    };
}

#endif
