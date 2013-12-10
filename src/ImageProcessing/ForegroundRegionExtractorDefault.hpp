#ifndef FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP
#define FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

/*!
 * \brief   Image processing contains functionality for the different
 *          states of image processing required for human detection and tracking.
 * \details TODO
 */
namespace image_processing
{
/*!
 * \brief   Process step which does foreground modulation, creates bounding boxes.
 * \details TODO
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
     * \param settings TODO
     * \return         TODO
     */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
     * \brief        Performs the foreground modulation.
     * \details      TODO
     * \param frames TODO
     */
    void process(FrameList &frames) override;

private:
    cv::Mat foregroundMask;
    cv::vector<cv::vector<cv::Point> > contours;
    double minimalArea;
};
}

#endif
