#ifndef QUESEVERITYESTIMATOR_HPP
#define QUESEVERITYESTIMATOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Utilities/CameraObject.hpp"

#include <queue>

/*!
 * \brief   statistics contains functionality for extracting usefull data and meta data after
 *          the image processing steps have been completed.
 * \details TODO
 */
namespace statistics
{

/*!
 * \brief   A struct the containing information from each frame needed to determine queue status over time.
 * \details TODO
 */
struct FrameQueData
{
    /*!
     * \brief                   Constructor.
     * \param toalEnteredPeople TODO
     * \param queIsPresent      TODO
     */
    explicit FrameQueData(int totalEnteredPeople = 0, bool queIsPresent = false): totalEnteredPeople(totalEnteredPeople), queIsPresent(queIsPresent) {}

public:
    /*!
     * \brief The number of people that entered into view in this frame.
     */
    int totalEnteredPeople;
    
    /*!
      * \brief Whether a queue is visible in this frame or not.
      */
     bool queIsPresent;
};

/*!
 *  \brief      Process step that aggregates queue visibility over time and uses this information
 *              to output a stable classification of the current queue severity.
 *  \details    The calculated queue classification valu (0,1,2) is placed in the current frame object.
 */
class QueSeverityEstimator : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    QueSeverityEstimator();

    /*!
     * \brief          TODO
     * \details        TODO
     * \param settings TODO
     * \return         TODO
     */
    bool initialize( configuration::ConfigurationManager &settings ) override;

    /*!
     * \brief        TODO
     * \details      TODO
     * \param frames TODO
     */
    void process( FrameList & frames ) override;

private:
    std::deque<FrameQueData> historyWindow;
    int historyLength;
    double lowQueThreshold;
    double highQueThreshold;

    void shiftHistoryWindow( Frame newFrame );
    int classifyQueStatus();
};

}

#endif // QUESEVERITYESTIMATOR_HPP
