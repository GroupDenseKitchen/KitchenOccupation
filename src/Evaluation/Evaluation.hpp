#ifndef EVALUATION_H
#define EVALUATION_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

#include <numeric>


#include "../Configuration/ConfigurationManager.hpp"
#include "TrackerEvaluator.hpp"
 
/*!
 *  \brief   The evaluation namespace contains functionality for system evaluation.
 */
namespace evaluation
{

/*!
 * \brief     Evaluates system performance.
 * \details   System performance is evaluated by comparing system output to pre-labeled ground truth data.
 *            How performanced is measured is specified by the subalgorithm who inherit this class. Whhich sub algorithms
 *            that are to be used is specified in a configurationManager object that is passed to the initialize function.
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
     * \param settings Configuration-object containing the location of the ground truth files.
     * \return         True if successful.
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
     * \brief   Print stats to the debug log.
     */
    void printToLog();

private:

    std::vector<TrackerEvaluator*> trackingEvaluators;
    bool hasTrackerEvaluator;
    int trackEvalThreshold;

};

} // namespace evaluation

#endif // EVALUATION_H
