#ifndef FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP
#define FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

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
     *
     * \details        This algorithm has no configurable parameters.
     *
     * \return     True if successful.
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
