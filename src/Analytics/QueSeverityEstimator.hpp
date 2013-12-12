#ifndef QUESEVERITYESTIMATOR_HPP
#define QUESEVERITYESTIMATOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Utilities/CameraObject.hpp"

#include <queue>

namespace statistics
{

/*!
 * \brief   A struct the containing information from each frame needed to determine queue status over time.
 */
struct FrameQueData
{
    /*!
     * \brief                   Constructor.
     */
    explicit FrameQueData(int peopleInRoom = 0, bool queIsPresent = false): peopleInRoom(peopleInRoom), queIsPresent(queIsPresent) {}

public:
    /*!
     * \brief The number of people inside the room.
     */
    int peopleInRoom;
    
    /*!
      * \brief Whether a queue is visible in this frame or not.
      */
     bool queIsPresent;
};

/*!
 *  \brief      Process step that aggregates queue visibility over time and uses this information
 *              to output a stable classification of the current queue severity.
 *  \details    The calculated queue classification value (0,1,2) is placed in the current frame object.
 */
class QueSeverityEstimator : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    QueSeverityEstimator();

    /*!
     * \brief           Handles initialization.
     *
     * \details         Configurable algorithm parameters are:
     *                      - historyLength:            TODO: add description for this parameter here
     *                      - lowQueThreshold:          TODO: add description for this parameter here
     *                      - highQueThreshold:         TODO: add description for this parameter here
     *                      - lowOccupancyThreshold:    TODO: add description for this parameter here
     *                      - highOccupancyThreshold:   TODO: add description for this parameter here
     *
     *
     * \param settings  A configuration::ConfigurationManager object that potentially includes values for the
     *                  algorithm's constants.
     * \return          Returns false if the initialization fails, e.g. if a required variable is not set in the config file.
     */
    bool initialize( configuration::ConfigurationManager &settings ) override;

    /*!
     * \brief           Performs the algorithm.
     * \param frames    A FrameList object containing the current and some of the previos frames.
     */
    void process( FrameList & frames ) override;

private:
    std::deque<FrameQueData> historyWindow;
    int historyLength;
    double lowQueThreshold;
    double highQueThreshold;
    int lowOccupancyThreshold;
    int highOccupancyThreshold;


    void shiftHistoryWindow( Frame newFrame );
    int classifyQueStatus();
};

}

#endif // QUESEVERITYESTIMATOR_HPP
