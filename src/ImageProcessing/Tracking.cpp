#include "Tracking.hpp"

namespace image_processing
{
    Tracking::Tracking() {
        nextUniequeID = 1;
    }

    Tracking::~Tracking() {

    }

    bool Tracking::initialize(configuration::ConfigurationManager &config)
    {
        isInitialized = true;
        //TODO: Initialize variables
        return isInitialized;
    }

    void Tracking::process(FrameList &frames) {
        //TODO: loop over all cameras...)

        if(frames.hasPrevious())
        {
            CameraObject &cameraPrev = frames.getPrevious().getCameras().back();
            CameraObject &cameraCurr = frames.getCurrent().getCameras().back();

            populate(candidatePrev, cameraPrev.objects);
            populate(candidateCurr, cameraCurr.objects);

            while(!candidatePrev.empty() && !candidateCurr.empty())
                mapClosestCandidatePair(candidatePrev, candidateCurr, cameraPrev.objects, cameraCurr.objects);

            int lost = 0, found = 0;
            // Detected lost objects
            if(!candidatePrev.empty())
            {
                while(!candidatePrev.empty())
                {
                    //releaseID();
                    candidateCurr.pop_back();
                    lost++;
                }
            }

            // Found new objects
            if(!candidateCurr.empty())
            {
                while(!candidateCurr.empty())
                {
                    setID(cameraCurr.objects, candidateCurr.back().correspondingIndex, getUniqueID());
                    candidateCurr.pop_back();
                    found++;
                }
            }
        }
    }

    void Tracking::mapClosestCandidatePair(std::list<ObjectPair> & candidatePrev, std::list<ObjectPair> & candidateCurr, std::vector<Object> & prev, std::vector<Object> & curr)
    {
        std::list<ObjectPair>::iterator bestPrev, bestCurr;
        double shortestDistance = 1e10;

        for(std::list<ObjectPair>::iterator prevCandidate = candidatePrev.begin(); prevCandidate != candidatePrev.end(); prevCandidate++)
        {
            for(std::list<ObjectPair>::iterator currCandidate = candidateCurr.begin(); currCandidate != candidateCurr.end(); currCandidate++)
            {
                if(distance(*prevCandidate, *currCandidate) < shortestDistance)
                {
                    shortestDistance = distance(*prevCandidate, *currCandidate);
                    bestPrev = prevCandidate;
                    bestCurr = currCandidate;
                }
            }
        }
        bestCurr->object->id = bestPrev->object->id;
        candidatePrev.erase(bestPrev);
        candidateCurr.erase(bestCurr);
    }

    double Tracking::distance(ObjectPair previous, ObjectPair current)
    {
        double x1 = previous.object->boundingBox.x;
        double y1 = previous.object->boundingBox.y;
        double x2 = current.object->boundingBox.x;
        double y2 = current.object->boundingBox.y;
        return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
    }

    void Tracking::setID(std::vector<Object> & objects, int index, int ID)
    {
        std::vector<Object>::iterator object = objects.begin();
        for(int i = 0; i < index; i++)
            object++;
        object->id = ID;
    }

    int Tracking::getUniqueID()
    {
        return nextUniequeID++;
    }

    void Tracking::populate(std::list<ObjectPair> & objectPairs, std::vector<Object> & objects)
    {
        objectPairs.clear();
        int n = 0;
        for(int n = 0; n < objects.size(); n++)
        {
            objectPairs.push_back(ObjectPair(&objects[n], n));
        }
    }

}
