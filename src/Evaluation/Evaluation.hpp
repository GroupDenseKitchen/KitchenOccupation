#ifndef EVALUATION_H
#define EVALUATION_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

#include <numeric>


#include "../Configuration/ConfigurationManager.hpp"
#include "TrackerEvaluator.hpp"
 
/*!
 *  \brief   TODO
 *  \details TODO
 */
namespace evaluation
{

/*!
 * \brief     Evaluates system performance.
 * \details   Evaluates system performance by comparing system output and some intermediate steps
 *            in the pipeline to pre-labeled ground truth.
 */
class Evaluation : public Algorithm
{
public:

    /*!
     * \brief   Empty constructor
     */
    Evaluation();

    /*!
     * \brief   Destructor
     */
    ~Evaluation();

    /*!
     * \brief          Initializes the evaluation module
     * \details        TODO
     * \param settings Configuration-object containing the location of the ground truth files and other relevant settings.
     * \return         Returns true if successful.
     */
    bool initialize(configuration::ConfigurationManager& settings) override;

    /*!
     * \brief           Evaluates and updates the results for the system.
     * \details         Is called upon after each iteration in order to calculate the different performance metrics
     *                  by comparing the tracker system output to ground truth.
     * \param frameList The FrameList to be evaluated.
     */
    void process(FrameList& frames) override;

    /*!
     * \brief   Show stats in debug log.
     * \details TODO
     */
    void printToLog();

private:

    std::vector<TrackerEvaluator*> trackingEvaluators;
    bool hasTrackerEvaluator;
    int trackEvalThreshold;

};

} // namespace evaluation

#endif // EVALUATION_H
