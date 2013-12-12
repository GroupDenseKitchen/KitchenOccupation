#ifndef TRACKING_BRUTE_FORCE_HPP
#define TRACKING_BRUTE_FORCE_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "EntryExitCounter.hpp"

/*!
 * \brief   Image processing contains functionality for the different
 *          states of image processing required for human detection and tracking.
 * \details TODO
 */
namespace image_processing
{

/*!
 * \brief   Process step which tracks bounding boxes between frames.
 * \details TODO
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
     *              - maximumDistance:                 TODO: add description for this parameter here
     *              - minimumLifeSpan:                TODO: add description for this parameter here
     *              - maximumTimeLostInDoorArea:               TODO: add description for this parameter here
     *              - maximumTimeLostStill:                   TODO: add description for this parameter here
     *
     * \return     True if successful.
     */
    bool initialize(configuration::ConfigurationManager &settings) override;

    /*!
     * \brief        Performs the tracking.
     * \details      TODO
     * \param frames TODO
     */
    void process(FrameList &frames) override;

private:
    double maximumDistance; // Euclidian distance squared, in pixels
    int nextUniequeID;
    int minimumLifeSpan;    // # Frames
    int maximumTimeLost;    // # Frames
    int maximumTimeLostStill;
    int maximumTimeLostInDoorArea; //#Frames

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
