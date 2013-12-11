#ifndef TRACKER_EVALUATOR_H
#define TRACKER_EVALUATOR_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../ExtLib/rapidxml.hpp"
#include <numeric>

using namespace std;

namespace evaluation
{
/*!
 * \brief   Tracker accuracy evaluator
 * \details Evaluates the performance of the tracker by comparing the output to
 *          ground truth data. Performance is measured using the MOTA and MOTP
 *          methods described in 2008 by Bernardin, K. and Stiefelhagen, R. in their paper:
 *
 *          "Evaluating Multiple Object Tracking Performance: The CLEAR MOT Metrics".
 *          Interactive Systems Lab, Institut für Theoretische Informatik,
 *          Universität Karlsruhe, 76131 Karlsruhe, Germany.
 *
 *
 */
class TrackerEvaluator
{
public:
    /*!
     * \brief   Empty Constructor.
     */
    TrackerEvaluator();

    /*!
     * \brief   Destructor.
     */
    ~TrackerEvaluator();

    /*!
     * \brief                    Initializes the tracking module.
     * \details                  The Tracker evaluator is initialized by reading the ground truth file using the
     *                           rapidxml library.
     * \param groundTruthPath    Path to ground truth xml-file.
     * \param precisionThreshold Threshold for the MOTA/MOTP measurements (see paper).
     * \return                   Returns true if successful.
     */
    bool initialize(std::string groundTruthPath, int precisionThresh);

    /*!
     * \brief     Calculates MOTA and MOTP for the CameraObject.
     * \param cam The CameraObject in question.
     */
    void process(CameraObject &cam);

    /*!
     * \brief          Prints the MOTA and MOTP values ro the debug log.
     * \param camIndex Index of the current camera.
     */
    void printToLog(unsigned int camIndex);

private:
    int frameCounter, numberOfFrames, frameMismatches;
    int precisionThreshold, obID, hypID, obX, obY, hypX, hypY;
    float frameDistance, motpValue, motaValue;
    float sumDistance;
    int sumMisses, sumMatches, sumFalsePositive, sumMismatches, sumNumberOfObjects;

    vector<int> numberOfObjects, matches, misses, falsePositive, mismatches;
    vector<float> distance;
    vector<map<int, int>> correspondance;
    vector<vector<Object>> groundTruth;
    vector<Object> hypothesisList;

    CameraObject* camera;

    Object *ob, *hyp;
    Object* getObj(vector<Object>* objVec, int ID);

    void deleteObj(vector<Object>* objVec, int ID);
    bool isCorr(int truID, int hypID);

    void MOTP();
    void MOTA();

    bool readXML2FrameList(const char *fileName);
};


} // namspace evaluation



#endif // TRACKER_EVALUATOR_H
