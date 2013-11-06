#ifndef ENTRYEXITCOUNTER_HPP
#define ENTRYEXITCOUNTER_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"


namespace image_processing
{
/*!
 *  \brief     Process step which determines if objects are lost in an entry area, creates bounding boxes.
 */

class EntryExitCounter : public Algorithm
{
public:

    /*!
       \brief   Constructor.
    */
    EntryExitCounter();

    /*!
       \brief   Destructor.
    */
    ~EntryExitCounter();

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
    double yMax;
    double yMin;
    double xMin;  //should mabey be done in some other way, some mask or something but at "debug mode"
    double xMax; //needs to be camera specific

};
}
#endif
