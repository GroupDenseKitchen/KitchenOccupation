#ifndef QUEDETECTOR_HPP
#define QUEDETECTOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Analytics/Que.hpp"
#include "../Utilities/CameraObject.hpp"


namespace statistics
{

/*!
 *  \brief      Process step which uses information about visible persons' position and velocity
 *              and determines if a que is present or not.
 *  \details    A cubic bezier spline is fit to every pair of visible object/persons
 *              (one spline per direction) for each camera.
 *              The objects are used as the two on-curve control points and the objects' velocities
 *              are used to calculate the two off-curve control points. Objects that are connected by
 *              a sufficiently short spline are considered to be in the same queue. If any queues are found,
 *              this is indicated in the camera object for which the queue is visible. The class is written
 *              such that it would be easy to extend it so to be able to do more advanced analysis of the
 *              queue structures.
 */
class QueDetector : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    QueDetector();

    /*!
     * \brief           Handles initialization.
     *
     * \details         Configurable algorithm parameters are:
     *                      - maxSplineSegmentLength:       Spline subdivision stops when the longest segment
     *                                                      in the spline drops below this length (in pixels).
     *                      - velocityScaleFactor:          The scale factor applied to the velocity vectors
     *                                                      when creating a spline. (To account for spline
     *                                                      parametrization)
     *                      - splineLengthThreshold:        Pairs of objects connected by a spline longer than
     *                                                      this are not considered part of the same queue.
     *                      - maxRecursionDepth:            Spline subdivisions are stopped at this depth of recursions.
     *
     * \param settings  A configuration::ConfigurationManager object that potentially includes values for the
     *                  algorithm's constants.
     * \return          Returns false if the initialization fails, e.g. if a required variable is not set in the config file.
     */
    bool initialize( configuration::ConfigurationManager &settings ) override;

    /*!
     * \brief           Performs the algorithm.
     * \param frames    A FrameList object containing the current and some of the previos frames.
     */
    void process( FrameList & frames ) override;

private:
    double splineLengthThreshold;
    double velocityScaleFactor;
    double maxSplineSegmentLength;
    int maxRecursionDepth;
    double momentaryFps;

    void drawQue( cv::Mat & dstImage, const Que & que );
    void splineFromObjects( std::vector<Object> & objects , std::vector<SplineStrip>& spline, float maxSegmentLength );
    void subdivideSpline( SplineStrip& strip, std::vector<SplineStrip>& spline, float maxSegmentLength, int recursionDepth = 0);
    float splineLength( std::vector<SplineStrip> & );
    void detectQues( std::vector<Object> & objects, std::vector<Que> & ques );
    void extractFeasibleDestinations( Object & fromObject, std::vector<Object> & allObjects, std::vector<Object> & destObjects );
    void findBestDestination( Object & fromObject, std::vector<Object> & destObjects, DirectedQueEdge & destEdge );
    void quesFromEdges( std::vector<DirectedQueEdge> & queEdges, std::vector<Que> & ques );
};

}

#endif // QUEDETECTOR_HPP
