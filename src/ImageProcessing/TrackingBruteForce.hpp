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
    struct ObjectPair
    {
        Object * object;
        int correspondingIndex;
        ObjectPair(Object * object, int correspondingIndex) : object(object), correspondingIndex(correspondingIndex) {}
    };

private:
    std::list<ObjectPair> candidatePrev;
    std::list<ObjectPair> candidateCurr;
    int nextUniequeID;

    void populate(std::list<ObjectPair> & objectPairs, std::vector<Object> & objects);
    void mapClosestCandidatePair(std::list<ObjectPair> & candidatePrev, std::list<ObjectPair> & candidateCurr, std::vector<Object> & prev, std::vector<Object> & curr);
    int getUniqueID();
    void setID(std::vector<Object> & objects, int index, int ID);
    double distance(ObjectPair previous, ObjectPair current);
};
}

#endif
