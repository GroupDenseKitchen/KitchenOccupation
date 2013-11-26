#ifndef OPTICALFLOWSEGMENTATION_HPP
#define OPTICALFLOWSEGMENTATION_HPP

#include "../Utilities/Algorithm.hpp"
#include "../Utilities/VectorUtilities.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <algorithm>
#include <math.h>
#include <QDebug>

namespace image_processing{

struct FlowVector{
    cv::Point2f pos;
    cv::Point2f flow;
};

class OpticalFlowSegmentation : public Algorithm
{
public:
    OpticalFlowSegmentation();
    ~OpticalFlowSegmentation();

    void process(FrameList &frames) override;

    bool initialize(configuration::ConfigurationManager &settings) override;
private:
    void getOpticalFlow(cv::Mat current, cv::Mat prev);
    void globalOpticalFlow(cv::Mat current, cv::Mat prev);
    void paintFlowVectors(cv::Mat image, std::vector<FlowVector> flowVectors);
    void paintFlowVectors(cv::Mat image, std::vector<FlowVector> flowVectors, cv::Scalar color);
    void computeOpticalFlow(Frame current, Frame previous);
    void clusterVectors(const std::vector<FlowVector> &flowVectors);
    void drawOptFlowMap(const cv::Mat &flow, cv::Mat &cflowmap, int step, double, const cv::Scalar &color);

    float angleToXaxis(cv::Point2f vector);

    int maxPointsToTrack;
    int minPointsToTrack;

    std::vector<FlowVector> filterSmallest(std::vector<FlowVector> flowVectors);
    std::vector<FlowVector> averageFlow(std::vector<FlowVector> flowVectors, cv::Size imageSize);
    std::vector<FlowVector> lastFlowVectors;
    std::vector<cv::Point2f> lastTrackedPoints;
    std::vector<cv::Point2f> keyframeCoordinates;

    cv::FeatureDetector* detector;
    cv::Mat imageToDrawOn;
};
}
#endif // OPTIALFLOWSEGMENTATION_HPP
