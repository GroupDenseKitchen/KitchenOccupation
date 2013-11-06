#ifndef FLOWESTIMATOR_HPP
#define FLOWESTIMATOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"


namespace statistics  // ??? vad ska det stå här
{
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

     double inFlow;
     double outflow;
    //some vectors dependent on how many rooms...?
     //where do/should we set roomID?

};
}
#endif







