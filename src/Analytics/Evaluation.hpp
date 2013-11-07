#ifndef EVALUATION_H
#define EVALUATION_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"

#include "../Configuration/ConfigurationManager.hpp"
#include "TrackerEvaluator.hpp"

using namespace std;
using namespace cv;

/*!
 *  \brief     Evaluates system performance.
 *  \details   Evaluates system performance by comparing system output and some intermediate steps
 *             in the pipeline to pre-labeled ground truth.
 */

namespace evaluation
{

class Evaluation
{
public:

    /*!
     * \brief Empty constructor
     */
    Evaluation();

    /*!
     * \brief Destructor
     */
    ~Evaluation();

    /*!
     * \brief           Initializes the evaluation module
     * \param settings  Configuration-object containing the location of the ground truth files and other relevant settings.
     * \return          Returns true if successful.
     */
    bool initialize(configuration::ConfigurationManager& settings);

    /*!
     * \brief Evaluates and updates the results for the system.
     * \param frameList The FrameList to be evaluated.
     * \details Is called upon after each iteration in order to calculate the different performance metrics
     *          by comparing the tracker system output to ground truth.
     */
    void process(FrameList& frames);

    /*!
     * \brief Show stats in debug log.
     */
    void printToLog();

private:

    std::vector<TrackerEvaluator*> trackingEvaluators;

    int trackEvalThreshold;

};

} // namespace evaluation

#endif // EVALUATION_H
