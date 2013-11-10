#ifndef QUEDETECTOR_HPP
#define QUEDETECTOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Utilities/Que.hpp"
#include "../Utilities/CameraObject.hpp"

/*!
 *  \brief   Image processing contains functionality for the different
 *           states of image processing required for human detection and tracking.
 */
namespace image_processing
{


class QueDetector : public Algorithm
{
public:
    QueDetector();

    void process( FrameList & frames ) override;
    bool initialize( configuration::ConfigurationManager &settings ) override;

private:
    double drawStepSize;
    double maxSplineSegmentLength;
    double splineLengthThreshold;

    void drawQue( cv::Mat & dstImage, const Que & que );
    void splineFromObjects( std::vector<Object> & objects , std::vector<SplineStrip>& spline, double maxSegmentLength );
    void subdivideSpline( SplineStrip& strip, std::vector<SplineStrip>& spline, double maxSegmentLength );
    double splineLength( std::vector<SplineStrip> );
    void detectQues( std::vector<Object> & objects, std::vector<Que> & ques );
    void extractFeasibleDestinations( Object & fromObject, std::vector<Object> & allObjects, std::vector<Object> & destObjects );
    void findBestDestination( Object & fromObject, std::vector<Object> & destObjects, DirectedQueEdge & destEdge );
    void quesFromEdges( std::vector<DirectedQueEdge> & queEdges, std::vector<Que> & ques );
};

}

#endif // QUEDETECTOR_HPP
