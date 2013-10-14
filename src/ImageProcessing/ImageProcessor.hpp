#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "BackgroundModel.hpp"
#include "ForegroundRegionExtractor.hpp"
#include "Tracking.hpp"

/*!
 *  \brief   Image processing contains functionality for the different
 *           states of image processing required for human detection and tracking.
 */
namespace image_processing
{
/*!
 *  \brief     The Image Processor is the interface to the image processing functionality.
 *  \version   0.1
 *  \date      2013-10-10
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
       \brief   Initialize the ImageProcessor.
       \details Configurates which algorithms to be applied in which order.
    */
    bool initialize(configuration::ConfigurationManager & config) override;

    /*!
       \brief   Perform all image processing algorithms in order.
    */
    void process(FrameList &frames) override;

private:
};
}

#endif
