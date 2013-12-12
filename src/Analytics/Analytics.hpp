#ifndef STATISTICS_MODULE_H
#define STATISTICS_MODULE_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

/*!
 * \brief    Statistical analysis
 * \details  Estimates the number of people, models, flows etc..
 */
namespace statistics
{
/*!
 * \brief   The Analytics is the interface to statistical analysis of Frames.
 * \details TODO
 * \version 0.1
 * \date    2013-10-07
 */
class Analytics : public Algorithm
{
    public:
        /*!
         * \brief   Constructor.
         */
        Analytics();

        /*!
         * \brief   Destructor.
         */
        ~Analytics();

        /*!
         * \brief          Initialize the Analytics module.
         * \details        Configurates which algorithms to be applied in which order.
         *
         * \details        This algorithm acts as an interface to a set of subalgorithms and has no configurable parameters.
         *
         * \param settings TODO
         * \return         True if successful.
         */
        bool initialize(configuration::ConfigurationManager & settings) override;

        /*!
         * \brief   TODO
         * \details TODO
         */
        void reset();

        /*!
         * \brief        Perform all Statistics algorithms in order.
         * \details      TODO
         * \param frames TODO
         */
        void process(FrameList &frames) override;

    private:

};
}

#endif
