#ifndef FLOWESTIMATOR_HPP
#define FLOWESTIMATOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"


namespace statistics
{

/*!
 *  \brief     Stuct which is used for a vector with pair values.
 */
struct flowVectorPair {
    flowVectorPair(int _flow,unsigned int _frameCount);
    int flow;
    unsigned int frameCount;
};


/*!
 *  \brief     Process step which calculates the flow in  and out through the door.
 */
class FlowEstimator : public Algorithm
{ 
public:
    /*!
       \brief   Constructor.
    */
    FlowEstimator();

    /*!
       \brief   Destructor.
    */
    ~FlowEstimator();
    /*!
       \brief   Performs the process step.
    */
    void process(FrameList &frames) override;
    /*!
       \brief   Initialize the algorithm.
       \details Returns false if initialization fails,
                e.g. if a required variable is not set
    */
     bool initialize(configuration::ConfigurationManager &conf) override;

private:


     std::deque<Frame> frames;

     unsigned int framesToKeep;
     unsigned int frameCounter;
     std::vector<flowVectorPair> inFlow;
     std::vector<flowVectorPair> outFlow;
};
}
#endif







