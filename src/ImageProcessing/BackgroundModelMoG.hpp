#ifndef BACKGROUNDMODELMOG_HPP
#define BACKGROUNDMODELMOG_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"


namespace image_processing
{
/*!
 * \brief   Class which creates binary image using OpenCV function BackgroundModelMoG2.
 * \details Z. Zivkovic. Improved adaptive Gausian mixture model for background subtraction.
 *          International Conference Pattern Recognition, UK, August, 2004.
 */
class BackgroundModelMoG : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    BackgroundModelMoG();

    /*!
     * \brief   Destructor.
     */
    ~BackgroundModelMoG();

    /*!
     * \brief        Performs the process step of the background-foreground segmentation.
     * \details      Creates a binary image, "foregroundMask", and performs erotions and dilations on the generated binary image.
     * \param frames Requires that each camera's latest frame contains a RGB-image called "rawImage".
     */
    void process(FrameList &frames) override;

    /*!
     * \brief   Initialize the algorithm.
     * \details Returns false if initialization fails, e.g. if a required variables is not set.
     *
     * \details Configurable algorithm parameters are:
     *              - nMixtures:            Toggles the maximum number of mixture models per pixel.
     *              - backgroundRatio:      Threshold defining whether the component is significant
     *                                      enough to be included into the background model or not. For alpha=0.001,
     *                                      it means that the mode should exist for approximately 105 frames before
     *                                      it is considered foreground.
     *              - varThresholdGen:      Threshold for the squared Mahalanobis distance that helps decide when
     *                                      a sample is close to the existing components.
     *              - varThreshold:         Threshold for the squared Mahalanobis distance that helps decide when
     *                                      a sample is close to the existing components.
     *              - fVarInit:             Initial variance for the newly generated components.
     *                                      It affects the speed of adaptation. The parameter value is based on
     *                                      your estimate of the typical standard deviation from the images.
     *              - isShadowDetection:    Parameter defining whether shadow detection should be enabled
     *              - erotions:             Number of erotions.
     *              - dilations:            Number of dilations.
     *              - history:              Length of the history.
     *              - learningRate:         Parameter which decides the learning rate. Defined between 0 and 1.
     *              - downSamplingFactor:   Downsampling parameter. Shirinnks the image's sides with 1/downSamplingFactor.
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager &conf) override;

private:
    int cameraAmount;
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
    std::vector<cv::BackgroundSubtractorMOG2*> backgroundModels;

    void initializeBackgroundModels(int cameraAmount);
};
}

#endif
