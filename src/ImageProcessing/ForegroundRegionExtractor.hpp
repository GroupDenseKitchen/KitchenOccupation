#ifndef FOREGROUND_REGION_EXTRACTOR_HPP
#define FOREGROUND_REGION_EXTRACTOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Abstractalgorithm.hpp"

/*!
 *  \brief   Image processing contains functionality for the different
 *           states of image processing required for human detection and tracking.
 */
namespace image_processing
{
/*!
 *  \brief     TODO...
 *  \version   0.1
 *  \date      2013-10-10
 */
class ForegroundRegionExtractor : public AbstractAlgorithm
{
public:

    /*!
       \brief   Constructor.
    */
    ForegroundRegionExtractor();

    /*!
       \brief   Destructor.
    */
    ~ForegroundRegionExtractor();

    /*!
       \brief   Initializer.
    */
    bool initialize();

    /*!
       \brief   TODO...
    */
    void process(FrameList frames) override;

private:
};
}

#endif
