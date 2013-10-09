#ifndef STATISTICS_MODULE_H
#define STATISTICS_MODULE_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Abstractalgorithm.hpp"

/*!
 *  \brief    Statistical analysis
 *  \details  Estimates the number of people, models, flows etc..
 */
namespace statistics
{
/*!
 *  \brief     The Analytics is the interface to statistical analysis of Frames.
 *  \version   0.1
 *  \date      2013-10-07
 */
class Analytics : public AbstractAlgorithm
{
    public:
        /*!
           \brief   Constructor.
        */
        Analytics();

        /*!
           \brief   Destructor.
        */
        ~Analytics();

        /*!
           \brief   Process the current frame (possibly using older frames) by calling
                    statistical analytics algorithms in order.
        */
        void process(FrameList frames) override;
    private:

};
}

#endif
