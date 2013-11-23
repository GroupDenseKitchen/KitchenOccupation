#ifndef CIRCLEDETECTION_HPP
#define CIRCLEDETECTION_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Utilities/CameraObject.hpp"

/*!
 *  \brief   Image processing contains functionality for the different
 *           stages of image processing required for human detection and tracking.
 */
namespace image_processing
{

/*!
 *  \brief   Process step which detects circles in the image that are meant to be
 *           indicative of th presence of human heads.
 */
class CircleDetection : public Algorithm
{
public:
    /*!
       \brief  Constructor
    */
    CircleDetection();
    /*!
       \brief  Initialization of values used by the algorithm.
    */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
       \brief   Uses Canny edge detector on each of the image channels, combines them,
                and convolves the result with a kernel designed to detect circes and
                ellipses. The result is thresholded and high enough values are used as
                person hypotheses.
    */
    void process(FrameList &frames) override;

private:
    void makeCircleFilters(std::vector<cv::Mat> & filters, int circleThickness, int avgFilterSize);
    //void makeCircleFilters(std::vector<cv::Mat> & filters, int numCircles);

    int lowThreshold; //Low threshold for canny detection
    int highThreshold; //High threshold for canny detection
    int houghThreshold; // Threshold for when a head is detected
    int kernelSize; //Size of kernel used for bluring the image
    int averageCircleFilterSize;
    double downSamplingFactor; //Factor for downsapling the image for processing speed

    std::vector<cv::Mat> circleFilters; //Container of circle filters
    std::vector<int> circleFilterSizes = {35}; //TODO figure out how to initialize this with CONFIG
    int circleFilterRadiusDifference; //The disparity between the larges and smalles circle radius used in circle filters.
};

}
#endif // CIRCLEDETECTION_HPP
