#include "Tracking.hpp"

namespace image_processing
{
    Tracking::Tracking() {
        nextUniequeID = 1;
    }

    Tracking::~Tracking() {

    }

    bool Tracking::initialize(configuration::ConfigurationManager &configuration)
    {
        return true;
    }

    void Tracking::process(FrameList &frames) {
        //TODO: loop over all cameras...)
        if(frames.hasPrevious())
        {
            CameraObject cameraPrev = frames.getPrevious().getCameras().back();
            CameraObject cameraCurr = frames.getCurrent().getCameras().back();

            populate(candidatePrev, cameraPrev.objects);
            populate(candidateCurr, cameraCurr.objects);

            while(!candidatePrev.empty() && !candidateCurr.empty())
                mapClosestCandidate(candidatePrev, candidateCurr, cameraPrev.objects, cameraCurr.objects);

            // Detected lost objects
            if(!candidatePrev.empty())
            {
                while(!candidatePrev.empty())
                {
                    //releaseID();
                    candidateCurr.pop_back();
                }
            }

            // Found new objects
            if(!candidateCurr.empty())
            {
                while(!candidateCurr.empty())
                {
                    setID(cameraCurr.objects, candidateCurr.back().correspondingIndex, getUniqueID());
                    candidateCurr.pop_back();
                }
            }
        }
    }

    void Tracking::mapClosestCandidate(std::list<ObjectPair> & candidatePrev, std::list<ObjectPair> & candidateCurr, std::vector<Object> & prev, std::vector<Object> & curr)
    {
        int bestPrevIndex = 0, bestCurrIndex = 0;

        // Loopa över alla candidatePrev och candidateCurr,
        // hitta de som är närmast varandra (min(distance(...))
        // och sätt bestPrev och bestCurr till de motsvarande bästa objecten
        // Efter: sätt id:t på bestPrev till bestCurr
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
            n++;
        }
        /*
        for(std::vector<Object>::iterator object = objects.begin(); object != objects.end(); object++)
        {
            objectPairs.push_back(ObjectPair(&*object, n));
            n++;
        }
        */
    }

}
