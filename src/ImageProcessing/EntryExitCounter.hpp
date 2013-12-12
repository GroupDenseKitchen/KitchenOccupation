#ifndef ENTRYEXITCOUNTER_HPP
#define ENTRYEXITCOUNTER_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

/*!
 * \brief   TODO
 * \details TODO
 */
namespace image_processing
{
/*!
 * \brief       Checks if a 2D-point is inside a polygon.
 * \details     TODO
 * \param mask  TODO
 * \param point TODO
 * \return      TODO
 */
bool isInsidePolygon(cv::Mat mask, cv::Point2d point);

/*!
 * \brief   Process step which determines if objects are lost in an entry area, creates bounding boxes.
 * \details TODO
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
     * \details      TODO
     * \param frames TODO
     */
    void process(FrameList &frames);

    /*!
     * \brief      Initialize the algorithm.
     * \details    Returns false if initialization fails,
     *             e.g. if a required variable is not set.
     * \param conf TODO
     * \return     TODO
     */
    bool initialize(configuration::ConfigurationManager &conf);

private:
    int totalPopulation;
};
}
#endif
