#ifndef OPTICALFLOWSEGMENTATION_HPP
#define OPTICALFLOWSEGMENTATION_HPP

#include "../Utilities/Algorithm.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
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
    void paintFlowVectors(cv::Mat image, std::vector<FlowVector> flowVectors);
    void computeOpticalFlow(Frame current, Frame previous);
    void removeOutliers(std::vector<FlowVector> FlowVectors);

    cv::FeatureDetector* detector;

    std::vector<FlowVector> lastFlowVectors;
    std::vector<cv::Point2f> lastTrackedPoints;

    std::vector<cv::Point2f> keyframeCoordinates;
    cv::Mat imageToDrawOn;

    int maxPointsToTrack;
    int minPointsToTrack;
};
}
#endif // OPTIALFLOWSEGMENTATION_HPP
