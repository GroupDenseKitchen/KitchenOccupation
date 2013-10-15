#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

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
 *  \brief     The Image Processor is the interface to the image processing functionality.
 */
class ImageProcessor : public Algorithm
{
public:

    /*!
       \brief   Constructor.
    */
    ImageProcessor();

    /*!
       \brief   Destructor.
    */
    ~ImageProcessor();

    /*!
       \brief   Initialize all image processing algorithms.
    */
    bool initialize(configuration::ConfigurationManager & settings) override;

    /*!
       \brief   Perform all image processing algorithms in order.
    */
    void process(FrameList &frames) override;
};
}

#endif
