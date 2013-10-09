#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

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
 *  \brief     The Image Processor is the interface to the image processing functionality.
 *  \version   0.1
 *  \date      2013-10-07
 */
class ImageProcessor : public AbstractAlgorithm
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
       \brief   Process the current frame (possibly using older frames) by calling
                image processing algorithms in order.
    */
    void process(FrameList frames) override;
private:
};
}

#endif
