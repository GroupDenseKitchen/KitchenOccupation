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
 *  \brief     TODO...
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
       \brief   Initializer.
    */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
       \brief   TODO...
    */
    void process(FrameList &frames) override;

private:
    std::list<Object*> candidatePrev;
    std::list<Object*> candidateCurr;
    int nextUniequeID;

    double maximumDistance;

    void populate(std::list<Object*> & candidates, std::vector<Object> & objects);
    double mapClosestCandidatePair(std::list<Object*> & candidatePrev, std::list<Object*> & candidateCurr, std::vector<Object> & prev, std::vector<Object> & curr);
    int getUniqueID();
    double distance(Object* previous, Object* current);
};
}

#endif
