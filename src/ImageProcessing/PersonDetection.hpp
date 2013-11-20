#ifndef PERSONDETECTION_HPP
#define PERSONDETECTION_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Utilities/CameraObject.hpp"

/*!
 *  \brief   Image processing contains functionality for the different
 *           states of image processing required for human detection and tracking.
 */
namespace image_processing
{

class PersonDetection : public Algorithm
{
public:
    PersonDetection();

    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
       \brief  Explenation here
    */
    void process(FrameList &frames) override;

private:
    void calculateGradientMagnitude(cv::Mat & grayImg, cv::Mat & gradientMag);
    void calculatePointMatches();
    void makeCircleFilters(std::vector<cv::Mat> & filters, int numCircles);

    cv::Ptr<cv::FeatureDetector> detector;
    cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
    cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;

    int lowThreshold = 100;
    int highThreshold = 200;
    int minDist = 100;
    int kernelSize = 3;


    std::string detectorType;
    std::string descriptorType;
    std::string matcherType;

    cv::Mat previousImage;
    cv::Mat currentImage;
    cv::Ptr<std::vector<cv::KeyPoint> > previousKeyPoints;
    cv::Ptr<std::vector<cv::KeyPoint> > currentKeyPoints;
    cv::Mat previousDescriptors;
    cv::Mat currentDescriptors;
    std::vector<cv::Mat> circleFilters;

};

}
#endif // PERSONDETECTION_HPP
