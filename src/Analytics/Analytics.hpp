#ifndef STATISTICS_MODULE_H
#define STATISTICS_MODULE_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Abstractalgorithm.hpp"

namespace statistics{
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
