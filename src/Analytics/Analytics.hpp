#ifndef STATISTICS_MODULE_H
#define STATISTICS_MODULE_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

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
class Analytics : public Algorithm
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
           \brief   Initialize the Analytics module.
           \details Configurates which algorithms to be applied in which order.
        */
        bool initialize(configuration::ConfigurationManager & config) override;

        /*!
           \brief   Perform all Statistics algorithms in order.
        */
        void process(FrameList &frames) override;

    private:

};
}

#endif
