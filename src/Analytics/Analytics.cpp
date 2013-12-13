#include "Analytics.hpp"

namespace statistics
{
    Analytics::Analytics() {
        
    }

    Analytics::~Analytics() {
        
    }

    bool Analytics::initialize(configuration::ConfigurationManager & settings)
    {
        
        return Algorithm::initialize(settings);   // Initialize all algorithms
    }

    void Analytics::reset()
    {
        //TODO implement reset method if needed
    }

    void Analytics::process(FrameList &frames)
    {        
        Algorithm::process(frames);
    }

}
