#include "Analytics.hpp"

namespace statistics
{
    Analytics::Analytics() {
        //TODO stub
    }

    Analytics::~Analytics() {
        //TODO stub
    }

    bool Analytics::initialize(configuration::ConfigurationManager & settings)
    {
        //TODO: Add algorithms...
        return Algorithm::initialize(settings);   // Initialize all algorithms
    }

    void Analytics::process(FrameList &frames)
    {
        PROFILER_START("Statistics Processing");
        Algorithm::process(frames);
        PROFILER_END();
    }

}
