#ifndef BACKGROUNDMODELMOG_HPP
#define BACKGROUNDMODELMOG_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

namespace image_processing
{
/*!
 *  \brief     TODO:...
 *  \details   TODO:...
 */
class BackgroundModelMoG : public Algorithm
{
public:
    BackgroundModelMoG();
    ~BackgroundModelMoG();
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
