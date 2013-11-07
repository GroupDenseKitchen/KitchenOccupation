#ifndef TRACKER_EVALUATOR_H
#define TRACKER_EVALUATOR_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/rapidxml.hpp"

using namespace std;

namespace evaluation
{
/*!
 * \brief Tracker accuracy evaluator
 * \details Evaluates the performance of the tracker by comparing the output to
 *          ground truth data. Performance is measured using the MOTA and MOTP
 *          methods.
 */
class TrackerEvaluator
{
public:
    /*!
     * \brief Empty Constructor.
     */
    TrackerEvaluator();

    /*!
     * \brief Destructor.
     */
    ~TrackerEvaluator();

    /*!
     * \brief Initializes the tracking module.
     * \param groundTruthPath Path to ground truth xml-file.
     * \param precisionThreshold Threshold for the MOTA/MOTP measurements.
     * \return Returns true if successful.
     */
    bool initialize(std::string groundTruthPath, int precisionThresh);

    /*!
     * \brief Calculates MOTA and MOTP for the CameraObject.
     * \param camera The CameraObject in question.
     */
    void process(CameraObject &cam);

    /*!
     * \brief Prints the MOTA and MOTP values.
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
