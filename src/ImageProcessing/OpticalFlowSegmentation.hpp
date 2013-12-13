#ifndef OPTICALFLOWSEGMENTATION_HPP
#define OPTICALFLOWSEGMENTATION_HPP

#include "../Utilities/Algorithm.hpp"
#include "../Utilities/VectorUtilities.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <algorithm>
#include <math.h>

namespace image_processing{

struct FlowVector{
    cv::Point2f pos;
    cv::Point2f flow;
};


/*!
 * \brief       Computes the optical flow and does some basic segmentation based on it.
 * \details     The algorithm performs the following steps \n
 *              1) The algorithm converts the image to grayscale
 *              2) The algorithm runs a keypoint detector
 *              3) The algorithm tracks the points over a few frames
 *              4) Re-run the detector when to many points have been lost
 *              5) Average the flow vectors blockwise
 *              The algorithm does not:
 *              1) Compute an acual segmentation
 *              2) Run fast enough to be practical
 */
class OpticalFlowSegmentation : public Algorithm
{
public:
    /*!
     * \brief constructor
     */
    OpticalFlowSegmentation();
    /*!
     * \brief destructor
     */
    ~OpticalFlowSegmentation();

    /*!
     * \brief   The processing performs an optical flow calculation and averages the resulting vectors blockwise
     */
    void process(FrameList &frames) override;
    /*!
     * \brief      Initialize the algorithm.
     * \details    Returns false if initialization fails,
     *             e.g. if a required variable is not set.
     *
     *  \details   Configurable algorithm parameters are:
     *             This algorigthm does not have any configurable parameters
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager &settings) override;
private:
    void getOpticalFlow(cv::Mat current, cv::Mat prev);
    void globalOpticalFlow(cv::Mat current, cv::Mat prev);
    void paintFlowVectors(cv::Mat image, std::vector<FlowVector> flowVectors);
    void paintFlowVectors(cv::Mat image, std::vector<FlowVector> flowVectors, cv::Scalar color);
    void computeOpticalFlow(Frame current, Frame previous);


    std::vector<FlowVector> filterSmallest(std::vector<FlowVector> flowVectors);
    std::vector<FlowVector> averageFlow(std::vector<FlowVector> flowVectors, cv::Size imageSize);
    void clusterVectors(const std::vector<FlowVector> &flowVectors);

    float angleToXaxis(cv::Point2f _vector);

    cv::FeatureDetector* detector;

    std::vector<FlowVector> lastFlowVectors;
    std::vector<cv::Point2f> lastTrackedPoints;

    std::vector<cv::Point2f> keyframeCoordinates;
    cv::Mat imageToDrawOn;

    int maxPointsToTrack;
    int minPointsToTrack;
    void drawOptFlowMap(const cv::Mat &flow, cv::Mat &cflowmap, int step, double, const cv::Scalar &color);
};
}
#endif // OPTIALFLOWSEGMENTATION_HPP
