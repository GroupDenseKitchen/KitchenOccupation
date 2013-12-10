#include "TrackingBruteForceLines.hpp"

namespace image_processing
{
    TrackingBruteForceLines::TrackingBruteForceLines() {
        nextUniequeID = 1;
    }

    TrackingBruteForceLines::~TrackingBruteForceLines() {}

    bool TrackingBruteForceLines::initialize(configuration::ConfigurationManager& settings) {
        isInitialized = true;

        CONFIG(settings, maximumDistance, "TrackingMaximumDistance", 4000);
        CONFIG(settings, minimumLifeSpan, "TrackingMinimumLifeSpan", 30);   //Currently # Frames, should be in ms...
        CONFIG(settings, maximumTimeLostInDoorArea, "MaximumTimeLostInDoorArea", 9);   //Currently # Frames, should be in ms...
        CONFIG(settings, maximumTimeLostStill, "TrackingMaximumTimeLostStill", 200);


        return isInitialized;
    }

    void TrackingBruteForceLines::process(FrameList &frames) {

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
                addLost(prevObjects, cameraCurr.getObjects(), cameraCurr.getTransitionaryObjects(), cameraCurr.getImage("rawImage"),frames.getDoorMask());

                // 5) Elevate pervious candidate objects to real objects if they have lived long enough.
                elevatePotentialObjects(cameraCurr.getPotentialObjects(), cameraCurr.getObjects(),cameraCurr.getNewlyFoundObjects());


                // Debug
                if(!cameraCurr.hasImage("debugImage"))
                    cameraCurr.addImage("debugImage", cameraCurr.getImage("rawImage").clone());
                cv::Mat debugImage = cameraCurr.getImage("debugImage");
                std::string text = "";
                int fontFace = cv::FONT_HERSHEY_PLAIN;
                double fontScale = 1;
                int thickness = 1;
                for(std::vector<Object>::iterator object = cameraCurr.getPotentialObjects().begin(); object != cameraCurr.getPotentialObjects().end(); object++) {
                    cv::Point2d pos = object->centerOfMass;
                    text = "lifespan: " + std::to_string(object->lifeSpan);
                    putText(debugImage, text, pos, fontFace, fontScale, cv::Scalar(0,0,255), thickness, 8);
                    cv::rectangle(debugImage, object->boundingBox, cv::Scalar(0,0,255), 2);     // Debug
                    cv::circle(debugImage, object->centerOfMass, 15, cv::Scalar(0,0,100), -1);

                }
                for(std::vector<Object>::iterator object = cameraCurr.getObjects().begin(); object != cameraCurr.getObjects().end(); object++) {
                    cv::Point2d pos = object->centerOfMass;
                    text = "id: "+std::to_string(object->id);

                    if(object->lost) {
                        text += ", LOST lifespan: " + std::to_string(object->lifeSpan);
                        cv::rectangle(debugImage, object->boundingBox, cv::Scalar(255,0,0), 2);     // Debug
                        cv::circle(debugImage, object->centerOfMass, 15, cv::Scalar(100,0,0), -1);
                        putText(debugImage, text, pos, fontFace, fontScale, cv::Scalar(255, 0, 0), thickness, 8);
                    } else {
                        cv::rectangle(debugImage, object->boundingBox, cv::Scalar(0,255,0), 2);     // Debug
                        cv::circle(debugImage, object->centerOfMass, 15, cv::Scalar(0,100,0), -1);
                        putText(debugImage, text, pos, fontFace, fontScale, cv::Scalar(0, 255, 0), thickness, 8);
                    }
                }
            }
        }
    }

    cv::Vec3f TrackingBruteForceLines::calculateHomLine(cv::Point2f pointOne, cv::Point2f pointTwo){
        cv::Vec3f point1(pointOne.x, pointTwo.y, 1);
        cv::Vec3f point2(pointTwo.x, pointTwo.y, 1);
        cv::Vec3f theLine = point1.cross(point2); //line is calculated but not normalized.
        //Normalize line
        cv::Point2f normer(theLine[0],theLine[1]);
        float n = cv::norm(normer);
        if(theLine[2] > 0){
            theLine[0] = -theLine[0]/n;
            theLine[1] = -theLine[1]/n;
            theLine[2] = -theLine[2]/n;
            return theLine;
        }
        theLine[0] = theLine[0]/n;
        theLine[1] = theLine[1]/n;
        theLine[2] = theLine[2]/n;
        return theLine;
    }

    int TrackingBruteForceLines::sideOfLine(cv::Point2f point, cv::Vec3f theLine){
        cv::Vec3f thePoint(point.x, point.y, 1);
        float d = thePoint.dot(theLine);
        if(d > 0){          //----> point is on opposit side of line from origin
            return +1;
        }
        if(d < 0){
        return -1;          // ---> Same side as line
        }else{
            return 0;       //  point is on the line
        }
    }

    void TrackingBruteForceLines::pairAndPopulate(std::list<Object> & candidatePrev, std::list<Object> & candidateCurr, std::vector<Object> & destination) {
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
                bestCurr->lost = false;
                destination.push_back(*bestCurr);
                candidatePrev.erase(bestPrev);
                candidateCurr.erase(bestCurr);
            }
            error = smallestError;
        }
    }

    void TrackingBruteForceLines::elevatePotentialObjects(std::vector<Object> & candidates, std::vector<Object> & destination, std::vector<Object> & newlyFoundObjects) {
        std::vector<Object>::iterator candidate = candidates.begin();
        while(candidate != candidates.end()) {
            if(candidate->lifeSpan >= minimumLifeSpan) {
                candidate->id = getUniqueID();
                newlyFoundObjects.push_back(*candidate);//for counting objects
                destination.push_back(*candidate);
                candidates.erase(candidate);
            } else {
                candidate++;
            }
        }
    }

    void TrackingBruteForceLines::addNew(std::list<Object> & newObjects, std::vector<Object> & destination) {
        for(std::list<Object>::iterator iterO = newObjects.begin(); iterO != newObjects.end(); ++iterO){
	//for(Object & object : newObjects) {
	    Object object = *iterO;
            object.lifeSpan = 1;
            object.enter();
            destination.push_back(object);
        }
    }

    void TrackingBruteForceLines::addLost(std::list<Object> & lostObjects, std::vector<Object> & destination,std::vector<Object> & transitionaryObjects, cv::Mat image, cv::Mat mask) {
        for(std::list<Object>::iterator iterO = lostObjects.begin(); iterO != lostObjects.end(); ++iterO){
	//for(Object & object : lostObjects) {
	  Object object = *iterO;
            if(object.lost) {
                object.lifeSpan++;
            } else {
                object.lost = true;
                object.lifeSpan = 1;
                object.exitPoint = object.centerOfMass;
                object.exit();
            }
            int height = image.size().height;
            int width = image.size().width;
            if(!isInsideRemovalArea(object,mask,height,width) && object.lifeSpan < maximumTimeLostStill) { // check is if lost for too long (maximumTimeLostStill frames) ...
                destination.push_back(object);
            } else {
                if(object.lifeSpan < maximumTimeLostInDoorArea){ //even if in exit area save it for maximumTimeLostInDoorArea frames
                  destination.push_back(object);
                }
                if(object.lifeSpan > maximumTimeLostInDoorArea-1)
                    transitionaryObjects.push_back(object);
            }
        }
    }
    bool TrackingBruteForceLines::isCloseImageBorder(cv::Point2d point, int height, int width, int margin){
        return point.x > (width -margin) || point.x < (margin) || point.y > (height -margin) || point.y < margin;
    }

    bool TrackingBruteForceLines::isInsideRemovalArea(Object & object, cv::Mat mask, int height, int width)
    {
        return isInsidePolygon(mask,object.exitPoint) || isCloseImageBorder(object.exitPoint,height,width, 30);
    }

    int TrackingBruteForceLines::getUniqueID()
    {
        return nextUniequeID++;
    }

    double TrackingBruteForceLines::distance(Object & previous, Object & current)
    {
        double x1 = previous.centerOfMass.x;
        double y1 = previous.centerOfMass.y;
        double x2 = current.centerOfMass.x;
        double y2 = current.centerOfMass.y;
        return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
    }
}



