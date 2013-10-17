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
 *  \date      2013-10-10
 */

namespace evaluation
{

class Evaluation
{
public:
    Evaluation();
    ~Evaluation();

    bool initialize(configuration::ConfigurationManager& settings, FrameList* frameList, int threshold = 20);

    bool readXML2FrameList(const char *fileName);
    void currentFrame();
    void MOTP();
    void MOTA();

    std::string printStats();

    // GroundTruth
    vector<vector<Object>> groundTruth;
    // Reference to global framelist
    FrameList* frameList;
    vector<Object> hypothesisList;

private:
    int frameCounter, numberOfFrames, frameMismatches;
    int T, obID, hypID, obX, obY, hypX, hypY;
    float frameDistance, motpValue, motaValue;
    float sumDistance;
    int sumMisses, sumMatches, sumFalsePositive, sumMismatches, sumNumberOfObjects;
    vector<int> numberOfObjects, matches, misses, falsePositive, mismatches;
    vector<float> distance;
    vector<map<int, int>> correspondance;
    Object *ob, *hyp;

    Object* getObj(vector<Object>* objVec, int ID);
    void deleteObj(vector<Object>* objVec, int ID);
    bool isCorr(int truID, int hypID);

    Mat infoDisplayMatrix;
};

} // namespace evaluation

#endif // EVALUATION_H