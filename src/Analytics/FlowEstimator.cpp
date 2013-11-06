#include "FlowEstimator.hpp"


namespace statistics //vad ska det vara här??
{
FlowEstimator::FlowEstimator() {

}

FlowEstimator::~FlowEstimator() {

}

bool FlowEstimator::initialize(configuration::ConfigurationManager& settings) {
    isInitialized = true;

    return isInitialized;
}

void FlowEstimator::process(FrameList &frames)
{
    if(frames.hasPrevious())
    {

        for(int n = 0; n < frames.getCurrent().getCameras().size(); n++)
        {
            //get current exits and entrys and put in a specific roomID-vector?

        }
        // for all roomIDs calculate current in/out flow estimated on earlier measurements.
        //measure som time differance between changes to be able to determine the "flow"

    }
}
}
