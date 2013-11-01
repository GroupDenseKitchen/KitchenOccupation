#ifndef EVALUATION_H
#define EVALUATION_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Object.hpp"
#include "../Utilities/rapidxml.hpp"

#include "../Configuration/ConfigurationManager.hpp"

using namespace std;
using namespace cv;

/*!
 *  \brief     Evaluates system performance.
 *  \details   Compares system output to ground truth.
 *  \version   0.1
 *  \date      2013-11-01
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

    bool initialize(configuration::ConfigurationManager& settings, FrameList* frameList, int threshold = 20);

    /*!
     * \brief Evaluates the MOTA & MOTP values for each frame
     * \details Is called upon after each iteration in order to calculate MOTA and MOTP
     *          values in order to evaluate tracker performance.
     */
    void currentFrame();

    /*!
     * \brief Show stats in debug log.
     */
    void printToLog();

    /*!
     * \brief Sets the frameList variable.
     * \details Necessary if program is restarted or a new file is loaded.
     * \param frameList     Global framelist
     */
    void setFrameList(FrameList* frameList);

private:
    int frameCounter, numberOfFrames, frameMismatches;
    int T, obID, hypID, obX, obY, hypX, hypY;
    float frameDistance, motpValue, motaValue;
    float sumDistance;
    int sumMisses, sumMatches, sumFalsePositive, sumMismatches, sumNumberOfObjects;

    vector<int> numberOfObjects, matches, misses, falsePositive, mismatches;
    vector<float> distance;
    vector<map<int, int>> correspondance;
    vector<vector<Object>> groundTruth;
    vector<Object> hypothesisList;

    FrameList* frameList;

    Object *ob, *hyp;
    Object* getObj(vector<Object>* objVec, int ID);

    void deleteObj(vector<Object>* objVec, int ID);
    bool isCorr(int truID, int hypID);

    void MOTP();
    void MOTA();

    bool readXML2FrameList(const char *fileName);

};

} // namespace evaluation

#endif // EVALUATION_H
