#ifndef TRACKING_HPP
#define TRACKING_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"

#include <vector>

/*!
 *  \brief   Image processing contains functionality for the different
 *           states of image processing required for human detection and tracking.
 */
namespace image_processing
{

struct ObjectPair
{
    Object * object;
    int correspondingIndex;
    ObjectPair(Object * object, int correspondingIndex) : object(object), correspondingIndex(correspondingIndex) {}
};

/*!
 *  \brief     TODO...
 *  \version   0.1
 *  \date      2013-10-10
 */
class Tracking : public Algorithm
{
public:

    /*!
       \brief   Constructor.
    */
    Tracking();

    /*!
       \brief   Destructor.
    */
    ~Tracking();

    /*!
       \brief   Initializer.
    */
    bool initialize(configuration::ConfigurationManager &configuration) override;

    /*!
       \brief   TODO...
    */
    void process(FrameList &frames) override;

private:
    std::list<ObjectPair> candidatePrev;
    std::list<ObjectPair> candidateCurr;
    int nextUniequeID;

    void populate(std::list<ObjectPair> & objectPairs, std::vector<Object> & objects);
    void mapClosestCandidate(std::list<ObjectPair> & candidatePrev, std::list<ObjectPair> & candidateCurr, std::vector<Object> & prev, std::vector<Object> & curr);
    int getUniqueID();
    void setID(std::vector<Object> & objects, int index, int ID);
};
}

#endif
