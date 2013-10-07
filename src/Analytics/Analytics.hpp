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
 *  \brief
 *  \details
 *  \version   0.1
 *  \date      2013-10-07
 */
class Analytics : public AbstractAlgorithm
{
    public:
        Analytics();
        ~Analytics();
        void process(FrameList frames) override;
    private:

};
}

#endif
