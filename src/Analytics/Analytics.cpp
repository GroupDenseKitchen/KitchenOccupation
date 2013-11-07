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

    void Analytics::reset()
    {
        // TO maybe DO if there is something to reset
    }

    void Analytics::process(FrameList &frames)
    {        
        Algorithm::process(frames);
    }

}
