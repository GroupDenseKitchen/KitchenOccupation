#ifndef KINECTSEGMENTATION_HPP
#define KINECTSEGMENTATION_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"


namespace image_processing
{

/*!
 * \brief   Process step which creates binary image using OpenCV function KinectSegmentation2.
 * \details TODO
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
     * \brief        Performs the background modulation.
     * \details      TODO
     * \param frames TODO
     */
    void process(FrameList &frames) override;

    /*!
     * \brief      Initialize the algorithm.
     * \details    Returns false if initialization fails,
     *             e.g. if a required variable is not set.
     *
     *  \details   Configurable algorithm parameters are:
     *              - headHeightMargin:         TODO: add description for this parameter here
     *              - lowestDistanceOverFloor:  TODO: add description for this parameter here
     *              - distanceToCameraMargin:   TODO: add description for this parameter here
     *              - minimalHumanArea:         TODO: add description for this parameter here
     *              - minimalHeadArea:          TODO: add description for this parameter here
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager &conf) override;

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
