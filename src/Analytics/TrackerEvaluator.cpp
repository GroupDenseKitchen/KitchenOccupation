#include "TrackerEvaluator.hpp"

using namespace rapidxml;

namespace evaluation
{

TrackerEvaluator::TrackerEvaluator()
{

}

TrackerEvaluator::~TrackerEvaluator()
{

}


bool TrackerEvaluator::initialize(std::string groundTruthPath, int precisionThresh)
{
    bool initSuccess = false;

    precisionThreshold = precisionThresh;

    frameCounter = 0;
    camera = 0;

    motpValue = 0;
    motaValue = 1;
    sumDistance = 0;
    sumMisses = 0;
    sumFalsePositive = 0;
    sumMatches = 0;
    sumMismatches = 0;
    sumNumberOfObjects = 0;


    initSuccess = readXML2FrameList(groundTruthPath.c_str());
    if(!initSuccess)
//        LOG("Tracking Evaluation Error", "Failed to read ground truth file at: " << groundTruthPath)
    return initSuccess;
}

bool TrackerEvaluator::readXML2FrameList(const char* fileName)
{
    ifstream myfile(fileName);

    if(!myfile.is_open()) {
        return false;
    }
    xml_document<> doc;

    vector<char> buffer((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    doc.parse<0> (&buffer[0]);

    xml_node<>* framePointer;
    xml_node<>* objectListPointer;
    xml_node<>* objectPointer;
    xml_node<>* objectPropertyPointer;

    framePointer = doc.first_node()->first_node();

    // Extract numberof frames
    char* frameAttribute = doc.first_node()->last_node()->first_attribute()->value();
    numberOfFrames = atoi(frameAttribute);

    int objectID;
    int x, y, h, w;

    // Loop untill no frames are left
    while(framePointer != 0)
    {
        //frameNumber = atoi(framePointer->first_attribute()->value());
        groundTruth.push_back(vector<Object>());

        objectListPointer = framePointer->first_node();
        objectPointer = objectListPointer->first_node();

        // Loop untill no objects are left
        while(objectPointer != 0)
        {
            // Extract information about objects
            objectID = atoi(objectPointer->first_attribute()->value());
            groundTruth.back().push_back(Object());

            objectPropertyPointer = objectPointer->first_node("box");
            x = atoi(objectPropertyPointer->first_attribute("xc")->value());
            y = atoi(objectPropertyPointer->first_attribute("yc")->value());
            h = atoi(objectPropertyPointer->first_attribute("h")->value());
            w = atoi(objectPropertyPointer->first_attribute("w")->value());

            groundTruth.back().back().boundingBox.x = x - w/2;
            groundTruth.back().back().boundingBox.y = y - h/2;
            groundTruth.back().back().boundingBox.height = h;
            groundTruth.back().back().boundingBox.width = w;
            groundTruth.back().back().id = objectID;

            objectPointer = objectPointer->next_sibling();
        }

        framePointer = framePointer->next_sibling();
    }


    for (vector<vector<Object> >::iterator i = groundTruth.begin(); i != groundTruth.end(); i++)
    {
        numberOfObjects.push_back( (int)i->size() );
    }

    return true;
}

void TrackerEvaluator::printToLog(unsigned int cameraIndex)
{
  //  LOG("Evaluation Tracking", "MOTA value of camera " + to_string(cameraIndex) + " is: " + to_string(motaValue));
  //  LOG("Evaluation Tracking", "MOTP value of camera " + to_string(cameraIndex) + " is: " + to_string(motpValue));
}

// This is magic in it's purest form, and also proof that
// the madman and the genius have much in common.
void TrackerEvaluator::process(CameraObject& cam)
{
    this->camera = &cam;

    // in case there is more movie than groundtruth
    if (frameCounter < numberOfFrames)
    {
        // Init variables
        frameDistance = 0;
        frameMismatches = 0;
        // Insert empty map for current frame
        map<int,int> tempMap;
        correspondance.push_back(tempMap);

        // The first frame desn't have any previous frame
        if (frameCounter > 0)
        {
            // Alg part 1.1
            hypothesisList = camera->getObjects();
            // Check if old correspondances, in previous frame, are still valid
            // Use dist < T

            // Alg part 1.2
            for (map<int,int>::iterator i = correspondance.at(frameCounter - 1).begin(); i != correspondance.at(frameCounter - 1).end();)
            {
                // Alg part 1.3.1
                // Get Object position from GroundTruthFrameList

                // Object id
                // Alg part 1.3.2
                obID = i->first;
                // Hypothesis id
                // Alg part 1.3.3
                hypID = i->second;

                // isCorr also adds the distance between frames to the
                // total distance, and removes found correspondences from both vectors
                if( isCorr(obID, hypID) )
                {

                    // Alg part 1.4.1 - 1.4.4
                    correspondance.at(frameCounter).insert(pair<int,int>(obID, hypID));
                    // Remove correctly classified objects from the list and previous frame map.
                    deleteObj(&groundTruth.at(frameCounter), obID);
                    deleteObj(&hypothesisList, hypID);
                    // Remove matched correspondances from the previous map.
                    map<int,int>::iterator iTemp = i;
                    i++;
                    correspondance.at(frameCounter - 1).erase(iTemp);
                }
                else
                {
                    // Alg part 1.4.5
                    i++;
                }
            }
        }

        // Objects without correspondance
        // Find matching hypothesis, allowing only 1-1 match
        // Alg part 2.1"
        multimap<double, pair<int, int> > distMap;
        // Alg part 2.2"
        // Create map with the distance from all objects to all hypothesis
        for( vector<Object>::iterator truObj = groundTruth.at(frameCounter).begin(); truObj != groundTruth.at(frameCounter).end(); truObj++)
        {
            // Alg part 2.3

            for( vector<Object>::iterator hypObj = hypothesisList.begin(); hypObj != hypothesisList.end(); hypObj++)
            {
                // Alg part 2.3
                double distance = sqrt(std::pow(truObj->boundingBox.x - hypObj->boundingBox.x, 2) +
                                       std::pow(truObj->boundingBox.y - hypObj->boundingBox.y, 2));
                    if (distance < precisionThreshold)
                    {
                        //distMap.emplace(distance, make_pair(truObj->id, hypObj->id));
                    }
            }
        }

        // Go through the map to find the shortest distance-pairs
        // while only allowing 1-1 matching
        while (!distMap.empty())
        {
            // Alg part 3.1
            obID = distMap.begin()->second.first;
            hypID = distMap.begin()->second.second;

            // Alg part 3.2
            if (correspondance.at(frameCounter - 1).size() > 0)
            {
                if (correspondance.at(frameCounter - 1)[obID] != hypID)
                {
                    // Alg part 3.3
                    frameMismatches += 1;
                }
            }

            // Alg part 3.4.1
            correspondance.at(frameCounter).insert(pair<int,int>(obID, hypID));
            // Alg part 3.4.2
            deleteObj(&groundTruth.at(frameCounter), obID);
            // Alg part 3.4.3
            deleteObj(&hypothesisList, hypID);
            // Alg part 3.4.4
            frameDistance += (float)distMap.begin()->first;
            // Alg part 3.4.5
            for ( multimap<double, pair<int, int> >::iterator it = distMap.begin(); it != distMap.end();)
            {
                // Alg part 3.5
                if ( it->second.first == obID)
                {
                    // Alg part 3.6
                    // It is not allowed to remove the object that the iterator is pointing at
                    multimap<double, pair<int, int> >::iterator itTemp = it;
                    it++;
                    distMap.erase(itTemp);
                }
                else
                {
                    it++;
                }
            }
            // Alg part 3.7
            mismatches.push_back(frameMismatches);
        }

        // Alg part 4
        // Push frame variables to corresponding vector
        matches.push_back((int)correspondance.at(frameCounter).size());
        misses.push_back((int)groundTruth.at(frameCounter).size());
        falsePositive.push_back((int)hypothesisList.size());
        mismatches.push_back(frameMismatches);
        distance.push_back(frameDistance);

        frameCounter++;

        // Calculate MOTP & MOTA
        MOTP();
        MOTA();
    }
    else
    {
        frameCounter++;
    }
}

void TrackerEvaluator::MOTP()
{
    // Sum distance
    sumDistance = 0;
    for  (vector<float>::iterator i = distance.begin(); i != distance.end(); i++)
    {
        sumDistance += *i;
    }

    // Sum number of matches
    sumMatches = accumulate(matches.begin(), matches.end(), 0);
    // Calculate quotient
    if (sumMatches > 0)
    {
        motpValue = (float)sumDistance/sumMatches;
    }
}

void TrackerEvaluator::MOTA()
{
    // Sum misses
    sumMisses = accumulate(misses.begin(), misses.end(), 0);
    // Sum false positive
    sumFalsePositive = accumulate(falsePositive.begin(), falsePositive.end(), 0);
    // Sum mismatches
    sumMismatches = accumulate(mismatches.begin(), mismatches.end(), 0);
    // Sum total number of objets
    sumNumberOfObjects = accumulate(numberOfObjects.begin(), numberOfObjects.end(), 0);
    // Calculate quotient
    if (sumNumberOfObjects > 0)
    {
        motaValue = 1 - (float)(sumMisses + sumFalsePositive + sumMismatches)/sumNumberOfObjects;
    }
}

Object* TrackerEvaluator::getObj(vector<Object>* objVec, int ID)
{
    for (vector<Object>::iterator it = objVec->begin(); it != objVec->end(); it++)
    {
        if(it->id == ID)
            return &(*it);
    }
    return NULL;
}

void TrackerEvaluator::deleteObj(vector<Object>* objVec, int ID)
{
    for (vector<Object>::iterator it = objVec->begin(); it != objVec->end();)
    {
        if (it->id == ID)
        {
            vector<Object>::iterator itTemp = it;
            it++;
            objVec->erase(itTemp);
            return;
        }
        else
        {
            it++;
        }
    }
}

bool TrackerEvaluator::isCorr(int truID, int hypID)
{
    // Check if a object-hypothesis pair is within threshold limit
    ob = getObj(&groundTruth.at(frameCounter), truID);
    if (!ob)
        return false;

    hyp = getObj(&hypothesisList, hypID);
    if (!hyp)
        return false;

    obX = ob->boundingBox.x;
    obY = ob->boundingBox.y;
    hypX = hyp->boundingBox.x;
    hypY = hyp->boundingBox.y;
    float dist = (float) sqrt(pow(obX - hypX, 2) + pow(obY - hypY, 2));

    if (dist < precisionThreshold)
    {
        frameDistance += dist;
        return true;
    }
    return false;
}

} // namespace evaluation

