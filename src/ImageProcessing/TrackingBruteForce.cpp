#include "TrackingBruteForce.hpp"

namespace image_processing
{
    TrackingBruteForce::TrackingBruteForce() {
        nextUniequeID = 1;
    }

    TrackingBruteForce::~TrackingBruteForce() {}

    bool TrackingBruteForce::initialize(configuration::ConfigurationManager& settings) {
        isInitialized = true;

        CONFIG(settings, maximumDistance, "maximumDistance", 500);

        return isInitialized;
    }

    void TrackingBruteForce::process(FrameList &frames) {
        //TODO: loop over all cameras...)
        if(frames.hasPrevious())
        {
            CameraObject &cameraPrev = frames.getPrevious().getCameras().back();
            CameraObject &cameraCurr = frames.getCurrent().getCameras().back();
            int lost = 0, found = 0;
            double lastDistance = 0;

            // Copy object lists to temp lists
            populate(candidatePrev, cameraPrev.objects);
            populate(candidateCurr, cameraCurr.objects);

            // Pair closest points and remove from temp lists
            while(lastDistance <= maximumDistance && !candidatePrev.empty() && !candidateCurr.empty())
                lastDistance = mapClosestCandidatePair(candidatePrev, candidateCurr);

            // Detected lost objects
            if(!candidatePrev.empty())
            {
                while(!candidatePrev.empty())
                {
                    //releaseID();
                    candidatePrev.back()->exit();
                    cameraCurr.objects.push_back(*candidatePrev.back());
                    candidatePrev.pop_back();
                    lost++;
                }
            }

            // Found new objects
            if(!candidateCurr.empty())
            {
                while(!candidateCurr.empty())
                {
                    candidateCurr.back()->id = getUniqueID();
                    candidatePrev.back()->enter();
                    candidateCurr.pop_back();
                    found++;
                }
            }

            cv::Mat raw = cameraCurr.getImage("rawImage");  // Debug
            std::string text = "";
            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 1;
            int thickness = 1;
            for(std::vector<Object>::iterator object = cameraCurr.objects.begin(); object != cameraCurr.objects.end(); object++) {
                cv::Point2d pos = object->center;
                text = "id: "+std::to_string(object->id);
                putText(raw, text, pos, fontFace, fontScale, cv::Scalar::all(255), thickness, 8);
            }
        }
    }

    double TrackingBruteForce::mapClosestCandidatePair(std::list<Object*> & candidatePrev, std::list<Object*> & candidateCurr)
    {
        std::list<Object*>::iterator bestPrev, bestCurr;
        double shortestDistance = 1e99;

        for(std::list<Object*>::iterator prevCandidate = candidatePrev.begin(); prevCandidate != candidatePrev.end(); prevCandidate++)
        {
            for(std::list<Object*>::iterator currCandidate = candidateCurr.begin(); currCandidate != candidateCurr.end(); currCandidate++)
            {
                if(distance(*prevCandidate, *currCandidate) < shortestDistance)
                {
                    shortestDistance = distance(*prevCandidate, *currCandidate);
                    bestPrev = prevCandidate;
                    bestCurr = currCandidate;
                }
            }
        }
        if(shortestDistance <= maximumDistance) {
            (*bestCurr)->merge(*bestPrev);
            candidatePrev.erase(bestPrev);
            candidateCurr.erase(bestCurr);
        }
        return shortestDistance;
    }

    double TrackingBruteForce::distance(Object* previous, Object* current)
    {
        double x1 = previous->center.x;
        double y1 = previous->center.y;
        double x2 = current->center.x;
        double y2 = current->center.y;
        return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
    }

    int TrackingBruteForce::getUniqueID()
    {
        return nextUniequeID++;
    }

    void TrackingBruteForce::populate(std::list<Object*> & candidates, std::vector<Object> & objects)
    {
        candidates.clear();
        for(int n = 0; n < objects.size(); n++)
        {
            candidates.push_back(&(objects[n]));
        }
    }

}
