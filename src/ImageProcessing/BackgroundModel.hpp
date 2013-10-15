#ifndef BACKGROUNDMODEL_HPP
#define BACKGROUNDMODEL_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

namespace image_processing
{
/*!
 *  \brief     Short, one-line description.
 *  \details   Longer, more detailed description of the class and it's purpose.
 *  \version   0.1.1 // Not sure about this.
 *  \date      2013-09-26 // Last change (would be neat if this could be synced with git.)
 *  \bug       Any knowns bugs in this class are listed here
 *  \warning   Warnings go here (about how to not use the class etc. etc.)
 */
class BackgroundModel : public Algorithm
{
public:
    BackgroundModel();
    ~BackgroundModel();
    void process(FrameList &frames) override;
    bool initialize(configuration::ConfigurationManager &conf) override;

private:
    cv::BackgroundSubtractorMOG2 bg;
    int nmixtures;
    int dilations;
    int erotions;
    int history;
    double backgroundRatio;
    double varThresholdGen;
    double fVarInit;
    double fVarMin;
    double fVarMax;
    double fCT;
    double fTau;
    bool isShadowDetection;

};
}


#endif
