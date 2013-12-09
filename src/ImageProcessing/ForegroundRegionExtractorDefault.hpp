#ifndef FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP
#define FOREGROUND_REGION_EXTRACTOR_DEFAULT_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

/*!
 *  \brief   Image processing contains functionality for the different
 *           states of image processing required for human detection and tracking.
 */
namespace image_processing
{
/*!
 *  \brief     Process step which does foreground modulation, creates bounding boxes.
 */
class ForegroundRegionExtractorDefault : public Algorithm
{
public:

    /*!
       \brief   Constructor.
    */
    ForegroundRegionExtractorDefault();

    /*!
       \brief   Destructor.
    */
    ~ForegroundRegionExtractorDefault();

    /*!
       \brief   Initialize the algorithm.
       \details Returns false if initialization fails,
                e.g. if a required variable is not set in the config file.
    */
    bool initialize(configuration::ConfigurationManager &settings);

    /*!
       \brief   Performs the foreground modulation.
    */
    void process(FrameList &frames);

private:
    cv::Mat foregroundMask;
    cv::vector<cv::vector<cv::Point> > contours;
    double minimalArea;
};
}

#endif
