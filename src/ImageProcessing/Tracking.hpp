#ifndef TRACKING_HPP
#define TRACKING_HPP

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
 *  \date      2013-10-07
 */
class Tracking : public AbstractAlgorithm
{
public:

    /*!
       \brief   Constructor.
    */
    Tracking();

    /*!
       \brief   Destructor.
    */
    ~Tracking();

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
