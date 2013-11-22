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
    void makeCircleFilters(std::vector<cv::Mat> & filters, int numCircles);

    int lowThreshold;
    int highThreshold;
    int kernelSize;
    double downSamplingFactor;

    std::vector<cv::Mat> circleFilters;
    std::vector<int> circleFilterSizes = {35}; //TODO figure out how to initialize this with CONFIG
    int circleFilterRadiusDifference;
};

}
#endif // PERSONDETECTION_HPP
