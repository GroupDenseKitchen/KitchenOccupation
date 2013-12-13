#ifndef STATISTICS_MODULE_H
#define STATISTICS_MODULE_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

/*!
 * \brief    Statistical analysis
 * \details  Estimates the number of people, models, flows etc.
 */
namespace statistics
{
/*!
 * \brief   The Analytics is the interface to statistical analysis of Frames.
 * \details Performs higher level analysis of the frame data, like estimating queue length etc
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
         * \param settings Configuration settings for all subalgorithms
         * \return         True if successful.
         */
        bool initialize(configuration::ConfigurationManager & settings) override;

        /*!
         * \brief   Reset the analytics interface
         * \details Not currently implemented
         */
        void reset();

        /*!
         * \brief        Perform all Statistics algorithms in order.
         * \details      Pass the frame list to each analytics algorithm in order
         * \param frames
         */
        void process(FrameList &frames) override;

    private:

};
}

#endif
