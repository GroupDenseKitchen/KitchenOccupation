#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Abstractalgorithm.hpp"

/*!
 *  \brief   Image processing
 *  \details Contains functionality for the different states of image processing
 *           required for human detection and tracking.
 */
namespace image_processing
{
/*!
 *  \brief     Image Processor
 *  \details   The interface to the image processing functionality.
 *  \version   0.1
 *  \date      2013-10-07
 */
class ImageProcessor : public AbstractAlgorithm
{
public:
	ImageProcessor();
	~ImageProcessor();

    void process(FrameList frames) override;
private:
};
}

#endif
