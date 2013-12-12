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
 * \details TODO: DETAILED DESCRIPTION OF HOW THE TRACKER WORKS
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
     *              - maximumTimeLostStill:           The maximum time (in # frames) a object is allowed to be lost before it is forgotton.
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
     * \brief        Performs the tracking.
     * \details      TODO
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
