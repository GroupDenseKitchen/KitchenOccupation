#include "Analytics.hpp"

namespace statistics
{
    Analytics::Analytics() {
        //TODO stub
    }

    Analytics::~Analytics() {
        //TODO stub
    }

    bool Analytics::initialize(configuration::ConfigurationManager & configuration)
    {
        //TODO: Add algorithms...
        return Algorithm::initialize(configuration);   // Initialize all algorithms
    }


    void Analytics::process(FrameList &frames)
    {
        PROFILER_START("Statistics Processing");
        Algorithm::process(frames);
        PROFILER_END();
    }

}
