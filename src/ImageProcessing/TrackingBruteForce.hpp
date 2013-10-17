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
       \brief   Initializer, always returns true.
    */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
       \brief   Performs the tracking.
    */
    void process(FrameList &frames) override;

private:
    std::list<Object*> candidatePrev;
    std::list<Object*> candidateCurr;
    int nextUniequeID;

    double maximumDistance;

    void populate(std::list<Object*> & candidates, std::vector<Object> & objects);
    double mapClosestCandidatePair(std::list<Object*> & candidatePrev, std::list<Object*> & candidateCurr);
    int getUniqueID();
    double distance(Object* previous, Object* current);
};
}

#endif
