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

    void process(FrameList & frames) override;
    bool initialize(configuration::ConfigurationManager &settings) override;

private:

    void drawQue(cv::Mat & dstImage, Que & que);
    void drawSplineStrip(cv::Mat & dstImage, SplineStrip & splineStrip, double stepSize);
};

}

#endif // QUEDETECTOR_HPP
