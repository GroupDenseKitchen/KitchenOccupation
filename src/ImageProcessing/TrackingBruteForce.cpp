#include "TrackingBruteForce.hpp"

namespace image_processing
{
    TrackingBruteForce::TrackingBruteForce() {
        nextUniequeID = 1;
    }

    TrackingBruteForce::~TrackingBruteForce() {}

    bool TrackingBruteForce::initialize(configuration::ConfigurationManager& settings) {
        isInitialized = true;

        CONFIG(settings, maximumDistance, "TrackingMaximumDistance", 2500);
        CONFIG(settings, minimumLifeSpan, "TrackingMinimumLifeSpan", 10);   //Currently # Frames, should be in ms...
        CONFIG(settings, maximumTimeLost, "TrackingMaximumTimeLost", 10);   //Currently # Frames, should be in ms...

        return isInitialized;
    }

    void TrackingBruteForce::process(FrameList &frames) {

        if(frames.hasPrevious())
        {
            for(int n = 0; n < frames.getCurrent().getCameras().size(); n++)
            {
                CameraObject & cameraCurr = frames.getCurrent().getCameras()[n];
                CameraObject & cameraPrev = frames.getPrevious().getCameras()[n];

                // Containers that will be reduced in size when manipulated below
                std::list<Object> prevPotentialObjects(cameraPrev.getPotentialObjects().begin(), cameraPrev.getPotentialObjects().end());
                std::list<Object> prevObjects(cameraPrev.getObjects().begin(), cameraPrev.getObjects().end());
                std::list<Object> currCandidates(cameraCurr.getObjects().begin(), cameraCurr.getObjects().end());
                cameraCurr.getObjects().clear();

                // The purpose here is to fill cameraCurr.objects with new or old actual objects
                // and cameraCurr.potentialObjects with candidates that may be considered objects in the future

                // 1) Previous objects are paired with current candidate objects and moved to current object list.
                pairAndPopulate(prevObjects, currCandidates, cameraCurr.getObjects());

                // 2) Previous potential objects are paired with the remaining current candidate objects
                //    and moved to current potential object list.
                pairAndPopulate(prevPotentialObjects, currCandidates, cameraCurr.getPotentialObjects());

                // 3) Any remaining current candidate list is added as (new) current potential object.
                addNew(currCandidates, cameraCurr.getPotentialObjects());

                // 4) Any remaining previous objects are flagged as lost and added to current objects.
                addLost(prevObjects, cameraCurr.getObjects());

                // 5) Elevate pervious candidate objects to real objects if they have lived long enough.
                elevatePotentialObjects(cameraCurr.getPotentialObjects(), cameraCurr.getObjects());

                // 6) Remove lost objects that have been lost for too long.
                removeLostObjects(cameraCurr.getObjects());

                // Debug
                cv::Mat raw = cameraCurr.getImage("rawImage");
                std::string text = "";
                int fontFace = cv::FONT_HERSHEY_PLAIN;
                double fontScale = 1;
                int thickness = 1;
                for(std::vector<Object>::iterator object = cameraCurr.getPotentialObjects().begin(); object != cameraCurr.getPotentialObjects().end(); object++) {
                    cv::Point2d pos = object->center;
                    text = "lifespan: " + std::to_string(object->lifeSpan);
                    putText(raw, text, pos, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
                    cv::rectangle(raw, cameraCurr.getPotentialObjects().back().boundingBox, cv::Scalar(255,0,0), 2);     // Debug
                }
                for(std::vector<Object>::iterator object = cameraCurr.getObjects().begin(); object != cameraCurr.getObjects().end(); object++) {
                    cv::Point2d pos = object->center;
                    text = "id: "+std::to_string(object->id);
                    if(object->lost)
                        text += ", LOST lifespan: " + std::to_string(object->lifeSpan);
                    putText(raw, text, pos, fontFace, fontScale, cv::Scalar::all(255), thickness, 8);
                }
            }
        }
    }

    void TrackingBruteForce::pairAndPopulate(std::list<Object> & candidatePrev, std::list<Object> & candidateCurr, std::vector<Object> & destination) {
        // Pair closest points and remove from temp lists
        double error = 0;
        while(error <= maximumDistance && !candidatePrev.empty() && !candidateCurr.empty())
        {
            double smallestError = 1e99;
            std::list<Object>::iterator bestPrev, bestCurr;
            for(std::list<Object>::iterator previous = candidatePrev.begin(); previous != candidatePrev.end(); previous++)
            {
                for(std::list<Object>::iterator current = candidateCurr.begin(); current != candidateCurr.end(); current++)
                {
                    if(distance(*previous, *current) < smallestError)
                    {
                        smallestError = distance(*previous, *current);
                        bestPrev = previous;
                        bestCurr = current;
                    }
                }
            }
            if(smallestError <= maximumDistance) {
                bestCurr->merge(&*bestPrev);
                destination.push_back(*bestCurr);
                candidatePrev.erase(bestPrev);
                candidateCurr.erase(bestCurr);
            }
            error = smallestError;
        }
    }

    void TrackingBruteForce::elevatePotentialObjects(std::vector<Object> & candidates, std::vector<Object> & destination) {
        std::vector<Object>::iterator candidate = candidates.begin();
        while(candidate != candidates.end()) {
            if(candidate->lifeSpan >= minimumLifeSpan) {
                candidate->id = getUniqueID();
                destination.push_back(*candidate);
                candidates.erase(candidate);
            } else {
                candidate++;
            }
        }
    }

    void TrackingBruteForce::removeLostObjects(std::vector<Object> & objects) {
        std::vector<Object>::iterator object = objects.begin();
        while(object != objects.end()) {
            if(object->lost && object->lifeSpan > maximumTimeLost) {
                objects.erase(object);
            } else {
                object++;
            }
        }
    }

    void TrackingBruteForce::addNew(std::list<Object> & newObjects, std::vector<Object> & destination) {
        for(Object & object : newObjects) {
            object.lifeSpan = 1;
            object.enter();
            destination.push_back(object);
        }
    }

    void TrackingBruteForce::addLost(std::list<Object> & lostObjects, std::vector<Object> & destination) {
        for(Object & object : lostObjects) {
            if(object.lost) {
                object.lifeSpan++;
            } else {
                object.lifeSpan = 1;
                object.lost = true;
                object.exit();
            }
            destination.push_back(object);
        }
    }

    int TrackingBruteForce::getUniqueID()
    {
        return nextUniequeID++;
    }

    double TrackingBruteForce::distance(Object & previous, Object & current)
    {
        double x1 = previous.center.x;
        double y1 = previous.center.y;
        double x2 = current.center.x;
        double y2 = current.center.y;
        return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
    }
}
