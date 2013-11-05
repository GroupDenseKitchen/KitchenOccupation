#ifndef TRACKING_BRUTE_FORCE_HPP
#define TRACKING_BRUTE_FORCE_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

/*!
 *  \brief   Image processing contains functionality for the different
 *           states of image processing required for human detection and tracking.
 */
namespace image_processing
{

/*!
 *  \brief   Process step which tracks bounding boxes between frames.
 */
class TrackingBruteForce : public Algorithm
{
public:

    /*!
       \brief   Constructor.
    */
    TrackingBruteForce();

    /*!
       \brief   Destructor.
    */
    ~TrackingBruteForce();

    /*!
       \brief   Initialize the algorithm.
       \details Returns false if initialization fails,
                e.g. if a required variable is not set in the config file.
    */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
       \brief   Performs the tracking.
    */
    void process(FrameList &frames) override;

private:
    std::list<Object*> candidatePrev;
    std::list<Object*> candidateCurr;
    std::list<Object> additionalObjects;
    int nextUniequeID;

    double maximumDistance; // Euclidian distance squared, in pixels
    int minimumLifeSpan;    // # Frames
    int maximumTimeLost;    // # Frames

    /*
    void transferLongLivedObjects(std::vector<Object> & potentialObjects, std::vector<Object> & objects);
    void mergeClosestPairs(std::vector<Object> & previousObjects, std::vector<Object> & currentObjects);
    void populate(std::list<Object*> & candidates, std::vector<Object> & objects);
    double mapClosestCandidatePair(std::list<Object*> & candidatePrev, std::list<Object*> & candidateCurr);
    */

    void pairAndPopulate(std::list<Object> & candidatePrev, std::list<Object> & candidateCurr, std::vector<Object> & destination);
    void elevatePotentialObjects(std::vector<Object> & candidates, std::vector<Object> & destination, std::vector<Object> & newlyFoundObjects);
    void removeLostObjects(std::vector<Object> & objects,std::vector<Object> & transitionary_Objects);
    void addNew(std::list<Object> & newObjects, std::vector<Object> & destination);
    void addLost(std::list<Object> & lostObjects, std::vector<Object> & destination);

    int getUniqueID();
    double distance(Object& previous, Object& current);
};
}

#endif
