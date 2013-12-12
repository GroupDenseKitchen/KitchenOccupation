#ifndef CIRCLEDETECTION_HPP
#define CIRCLEDETECTION_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Utilities/CameraObject.hpp"

/*!
 * \brief   Image processing contains functionality for the different
 *          stages of image processing required for human detection and tracking.
 * \details TODO
 */
namespace image_processing
{

/*!
 * \brief      Process step which detects circles in the image that are meant to be
 *             indicative of the presence of human heads.
 * \details    The approach is based on [Gardel, A.; Bravo, I.; Jimenez, P.; Lazaro, J.L.;
 *             Torquemada, A. "Real Time Head Detection for Embedded Vision Modules",
 *             Intelligent Signal Processing, 2007. WISP 2007. IEEE International Symposium on,
 *             On page(s): 1 - 6]. Although effective for simple cases we did not find the approach
 *             to be as usefull as hinted in the paper for more complex senarios. The algorithm class
 *             is therefore not completed.
 */
class CircleDetection : public Algorithm
{
public:
    /*!
     * \brief   Constructor
     */
    CircleDetection();

    /*!
     * \brief          Initialization of values used by the algorithm.
     * \details        TODO
     * \param settings TODO
     * \return         TODO
     */
    bool initialize(configuration::ConfigurationManager &settings);

    /*!
     * \brief        Uses Canny edge detector on each of the image channels, combines them,
     *               and convolves the result with a kernel designed to detect circes and
     *               ellipses. The result is thresholded and high enough values are used as
     *               person hypotheses.
     * \details      TODO
     * \param frames TODO
     */
    void process(FrameList &frames);

private:
    void makeCircleFilters(std::vector<cv::Mat> & filters, int circleThickness, int avgFilterSize);
    //void makeCircleFilters(std::vector<cv::Mat> & filters, int numCircles);
    void gradientCircleVoting( cv::Mat &grayImage );

    int detectionThreshold; //Threshold for detecting circles
    int lowThreshold; //Low threshold for canny detection
    int highThreshold; //High threshold for canny detection
    int houghThreshold; // Threshold for when a head is detected
    int kernelSize; //Size of kernel used for bluring the image
    int averageCircleFilterSize;
    double downSamplingFactor; //Factor for downsapling the image for processing speed
    bool maskOutForeground; //Is to indicate if foreground should be masked out before detecting circles

    std::vector<cv::Mat> circleFilters; //Container of circle filters
    int circleFilterRadiusDifference; //The disparity between the larges and smalles circle radius used in circle filters.
};

}
#endif // CIRCLEDETECTION_HPP
