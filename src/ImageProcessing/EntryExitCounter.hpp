#ifndef ENTRYEXITCOUNTER_HPP
#define ENTRYEXITCOUNTER_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

namespace image_processing
{
/*!
 * \brief       Checks if a 2D-point is inside a polygon.
 * \details     The polygon is specified as a cv matrix in the mask parameter
 * \param mask  A cv::Mat with value 255 inside the polygon
 * \param point A point to check if it is inside the polygon
 * \return      true if inside
 */
bool isInsidePolygon(cv::Mat mask, cv::Point2d point);

/*!
 * \brief   Process step which determines if objects are lost in an entry area, creates bounding boxes.
 * \details Registers if objects enters and exits the room the objects has to fulfill some requirements.
 *          To be considered as entered, an object has to be created for the first time in the set door
 *          area and pass the three circle lines and be elevated to a real object. To be registered as
 *          exited an object has to be a real object and disappear inside the door area, while also at
 *          least once passed the three lines.
 */
class EntryExitCounter : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    EntryExitCounter();

    /*!
     * \brief   Destructor.
     */
    ~EntryExitCounter();

    /*!
     * \brief        Performs the process step, counts people.
     */
    void process(FrameList &frames) override;

    /*!
     * \brief      Initialize the algorithm.
     * \details    Returns false if initialization fails,
     *             e.g. if a required variable is not set.
     *
     * \details    This algorithm has no configurable parameters.
     */
    bool initialize(configuration::ConfigurationManager &conf) override;

private:
    int totalPopulation;
};
}
#endif
