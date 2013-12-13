#ifndef FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP
#define FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

namespace image_processing
{
/*!
 * \brief   Process step which does foreground modulation, creates bounding boxes.
 * \details Creates bounding boxes in a binary image using OpenCV's built in functions findContours and boundingRect.
 */
class ForegroundRegionExtractorDefault : public Algorithm
{
public:

    /*!
     * \brief   Constructor.
     */
    ForegroundRegionExtractorDefault();

    /*!
     * \brief   Destructor.
     */
    ~ForegroundRegionExtractorDefault();

    /*!
     * \brief          Initialize the algorithm.
     * \details        Returns false if initialization fails,
     *                 e.g. if a required variable is not set in the config file.
     *
     * \details Configurable algorithm parameters are:
     *              - minimalArea: Sets the minimal area for a bounding box.
     *
     * \return         True if successful.
     */
    bool initialize(configuration::ConfigurationManager &settings);

    /*!
     * \brief        Performs the foreground modulation.
     * \details      Process step which does foreground modulation, creates bounding boxes
     */
    void process(FrameList &frames);

private:
    cv::Mat foregroundMask;
    cv::vector<cv::vector<cv::Point> > contours;
    double minimalArea;
};
}

#endif
