#ifndef KINECTSEGMENTATION_HPP
#define KINECTSEGMENTATION_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"


namespace image_processing
{

/*!
 * \brief       Require a depth image "rawImage" in the current frame for each camera, where darker is closer and black means undefined. Produces a vector of detected objects stored in the current frame for each camera.
 * \details     The algorithm perform the following steps:\n
 *                  1) The depth image is converted to gray scale and inverted except for the black color (and some margin: distanceToCameraMargin). Now darker is further away.\n
 *                  2) All values below lowestDistanceOverFloor is set to zero (black).\n
 *                  3) Contours are found in the image, any contour with an area less than minimalHumanArea is ignored.\n
 *                  4) Opening, Closening and Gaussian blurring is performed.\n
 *                  5) The maximum value in each contour is found and all values in respective contour less than headHeightMargin below the region max is set to zero, the rest form a combined binary mask.\n
 *                  6) Contours are found on the binary mask and each contour larger than minimalHeadArea is added as a potentially detected human to the current frame (of the processed camera), together with its center of mass and contour.
 */
class KinectSegmentation : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    KinectSegmentation();

    /*!
     * \brief   Destructor.
     */
    ~KinectSegmentation();

    /*!
     * \brief   The processing step of the Kinect Segmentation of human heads (and other large enough tall objects).
     */
    void process(FrameList &frames);

    /*!
     * \brief      Initialize the algorithm.
     * \details    Returns false if initialization fails,
     *             e.g. if a required variable is not set.
     *
     * \details    Configurable algorithm parameters are:
     *              - headHeightMargin:         The height interval croped from the top of the detected head and downwards. Affects the possibility of detecting shorter persons very close to long persons.
     *              - lowestDistanceOverFloor:  The limit of how short a person can be.
     *              - distanceToCameraMargin:   A margin length from the camera lens to the highest person possible to detect.
     *              - minimalHumanArea:         The minimal area required of a human, for it to be possible to detect.
     *              - minimalHeadArea:          The minimal area required of a human head, for it to be possible t detected.
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager &conf);

private:
    float maxIntensity;

    double headHeightMargin;
    int lowestDistanceOverFloor;
    int distanceToCameraMargin;
    int minimalHumanArea;
    int minimalHeadArea;

    void generateMaxRegionMask(cv::Mat & maxRegionMask, cv::Mat & grayImage, std::vector<std::vector<cv::Point>> & contours, int minimalArea, cv::Mat & debugImage);
    bool isInsidePolygon(cv::Mat mask, cv::Point2d point);
};

}

#endif
