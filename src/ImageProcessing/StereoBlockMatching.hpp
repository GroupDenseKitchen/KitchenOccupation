#ifndef STEREOBLOCKMATCHING_HPP
#define STEREOBLOCKMATCHING_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

namespace image_processing
{
/*!
 * \brief   Process step which creates a depth map from a stereo image pair.
 */
class StereoBlockMatching : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    StereoBlockMatching();

    /*!
     * \brief   Destructor.
     */
    ~StereoBlockMatching();

    /*!
     *  \brief          Performs stereo block matching using stereo block matching.
     *  \details        The stereo block matching algrothm used is the OpenCV semi-global block matcher (cv::StereoSGBM)
     *                  This algortihm is based on one defined by Hirschmuller, H in 2008.
     *
     *  \details        Hirschmuller, H. Stereo Processing by Semiglobal Matching and Mutual Information, PAMI(30), No. 2, February 2008, pp. 328-341
     *  \param frames   The current FrameList
     */
    void process(FrameList &frames);

    /*!
     * \brief      Initialize the algorithm.
     * \details    Returns false if initialization fails,
     *             e.g. if a required variable is not set.
     *
     * \details    This alghorithm has no configurable parameters.
     *
     * \return     True if successful
     */
    bool initialize(configuration::ConfigurationManager &conf);

private:
    int cameraAmount;
    std::vector<cv::BackgroundSubtractorMOG2*> backgroundModels;
    int nmixtures;
    int dilations;
    int erotions;
    int history;
    double backgroundRatio;
    double varThresholdGen;
    double varThreshold;
    double fVarInit;
    double fVarMin;
    double fVarMax;
    double fCT;
    double fTau;
    double learningRate;
    double downSamplingFactor;
    bool isShadowDetection;

    void initializeBackgroundModels(int cameraAmount);

};

}

#endif
