#ifndef TRACKING_BRUTE_FORCE_HPP
#define TRACKING_BRUTE_FORCE_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "EntryExitCounter.hpp"

namespace image_processing
{

/*!
 * \brief   Process step which tracks bounding boxes between frames.
 * \details The tracking algorithm performs six different steps for each frame iteration.
 *          The tracker has a list with objects from the last frame and a list with current
 *          objects found by earlier image processing steps.
 *          Objects are separated in different classes; candidate objects, lost objects and
 *          real objects. Candidate objects are objects that have been found but not lived
 *          long enough to be considered as a real object. Lost objects are objects that once
 *          were real objects but have been lost in the tracking. If an new object reappears
 *          close to the lost object it will be considered to be found and directly considered
 *          as a real object again. Real objects are the objects which has lived long enough
 *          and therefore considered as humans. Below the pipeline is described in six steps.
 *
 *          1) Check if the objects, received from the image processing made earlier in the pipeline, are inside any of the checkpoint masks and set flags. The checkpoint masks are the door area and circles which are set uped in the initial configuration.
 *          2) Previous objects from the last frame are paired with current candidate objects and moved to current object list. Pairing is done by relating the closest pairs to each other.
 *          3) A list with previous potential objects are paired with the remaining current candidate objects
 *          4) Any remaining current candidates in the list is added as (new) current potential object.
 *          5) Any remaining previous objects are flagged as lost. If one remaining object has been lost longer than a set maximum time, the object is removed.
 *          6) Elevate previous candidate objects to real objects if they have lived long enough.
 */
class TrackingBruteForce : public Algorithm
{
public:

    /*!
     * \brief   Constructor.
     */
    TrackingBruteForce();

    /*!
     * \brief   Destructor.
     */
    ~TrackingBruteForce();

    /*!
     * \brief          Initialize the algorithm.
     * \details        Returns false if initialization fails,
     *                 e.g. if a required variable is not set in the config file.
     *
     * \details Configurable algorithm parameters are:
     *              - maximumDistance:                The maximum distance a object can be considered to have moved since last frame.
     *              - minimumLifeSpan:                The minimal time (in # frames) a potential object must have existed (and been tracked) before it is considered a real object.
     *              - maximumTimeLostStill:           The maximum time (in # frames) a object is allowed to be lost before it is forgotten.
     *              - maximumTimeLostInDoorArea:      The maximum time (in # frames) a object is allowed to be lost in the door area before it is forgotten.
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
     * \brief        Performs the tracking.
     */
    void process(FrameList &frames) override;

private:
    double maximumDistance; // Euclidian distance squared, in pixels
    int nextUniequeID;
    int minimumLifeSpan;            // # frames
    int maximumTimeLost;            // # frames
    int maximumTimeLostStill;
    int maximumTimeLostInDoorArea;  // # frames

    void setIfIsInCheckpointRegion(std::list<Object> &objects, cv::Mat maskOne, cv::Mat maskTwo, cv::Mat maskThree);
    void pairAndPopulate(std::list<Object> & candidatePrev, std::list<Object> & candidateCurr, std::vector<Object> & destination);
    void elevatePotentialObjects(std::vector<Object> & candidates, std::vector<Object> & destination, std::vector<Object> & newlyFoundObjects);
    void removeLostObjects(std::vector<Object> & objects,std::vector<Object> & transitionary_Objects);
    void addNew(std::list<Object> & newObjects, std::vector<Object> & destination);
    void addLost(std::list<Object> & lostObjects, std::vector<Object> & destination,std::vector<Object> & transitionaryObjects,cv::Mat image ,cv::Mat mask);
    void writeToDebugImage(CameraObject & cameraCurr);
    bool isCloseImageBorder(cv::Point2d point, int height, int width, int margin);
    bool isInsideRemovalArea(Object & object, cv::Mat mask, int height, int width);
    int getUniqueID();
    double squaredDistance(Object& previous, Object& current);

};
}

#endif
