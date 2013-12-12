#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

/*!
 * \brief   Image processing contains functionality for the different
 *          states of image processing required for human detection and tracking.
 */
namespace image_processing
{
/*!
 * \brief   The Image Processor is the interface to the image processing functionality.
 * \details TODO
 */
class ImageProcessor : public Algorithm
{
public:

    /*!
     * \brief   Constructor.
     */
    ImageProcessor();

    /*!
     * \brief   Destructor.
     */
    ~ImageProcessor();

    /*!
     * \brief          Initialize all image processing algorithms.
     * \details        Returns false if any of the sub algorithms fail during their initialization.
     *
     * \details        This algorithm acts as an interface and has no configurable parameters:
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager & settings) override;

    /*!
     * \brief   TODO
     * \details TODO
     */
    void reset();

    /*!
     * \brief        Perform all image processing algorithms in order.
     * \details      TODO
     * \param frames TODO
     */
    void process(FrameList &frames) override;
};
}

#endif
