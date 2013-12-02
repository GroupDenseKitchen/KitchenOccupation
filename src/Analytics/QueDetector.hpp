#ifndef QUEDETECTOR_HPP
#define QUEDETECTOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Analytics/Que.hpp"
#include "../Utilities/CameraObject.hpp"

/*!
    \brief      statistics contains functionality for extracting usefull data and meta data after
                the image processing steps have been completed.
 */
namespace statistics
{

/*!
    \brief      Process step which uses information about visible persons' position and velocity
                and determines if a que is present or not.
 */
class QueDetector : public Algorithm
{
public:
    /*!
        \brief      Constructor.
     */
    QueDetector();

    /*!
        \brief      Handles initialization.
        \details Returns false if initialization fails,
                e.g. if a required variable is not set in the config file.
     */
    bool initialize( configuration::ConfigurationManager &settings ) override;

    /*!
        \brief      Performs the detection of ques.
        \details    Sets que visibility to true in each camera object where a que is visible.
     */
    void process( FrameList & frames ) override;

    //Theses are public for easier tuning but should be private in final version.
    double splineLengthThreshold;
    double velocityScaleFactor;
private:
    double maxSplineSegmentLength;
    int maxRecursionDepth;

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
