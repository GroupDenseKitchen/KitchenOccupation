#include "QueDetector.hpp"

namespace image_processing
{
    QueDetector::QueDetector()
    {
    }

    void QueDetector::process(FrameList &frames)
    {
        //TODO: FIX ME
        std::vector<CameraObject> cameras = frames.getCurrent().getCameras();

        if(!cameras[0].hasImage("rawImage"))
        {
            LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
            return;
        }
        //TODO: remove this stupid stuff
        //Dumb initializations
        Que dumbQue;
        dumbQue.splineStrips.push_back(SplineStrip(cv::Point2d(10,10),
                                                   cv::Point2d(20,20),
                                                   cv::Point2d(30,30),
                                                   cv::Point2d(40,40)));

        dumbQue.splineStrips.push_back(SplineStrip(cv::Point2d(100,100),
                                                   cv::Point2d(120,120),
                                                   cv::Point2d(130,130),
                                                   cv::Point2d(140,140)));

        cv::Mat image = cameras[0].getImage("rawImage").clone();
        drawQue(image,dumbQue);
        cameras[0].addImage("Splines",image);

    }

    void QueDetector::drawQue(cv::Mat &dstImage, Que & que)
    {

        for (SplineStrip sp : que.splineStrips) {
            drawSplineStrip( dstImage, sp, 0.1 ); //TODO: Move out constant
        }
    }

    bool QueDetector::initialize(configuration::ConfigurationManager &settings){ return true;} //TODO: Unhappy case

    void QueDetector::drawSplineStrip(cv::Mat & dstImage, SplineStrip & splineStrip, double stepSize)
    {
        int numPoints = (int)std::ceil(1/stepSize);
        cv::Mat_<cv::Point2d> splinePoints = cv::Mat_<cv::Point2d>(numPoints,1);
        cv::MatIterator_<cv::Point2d> iter = splinePoints.begin();
        int i = 0;
        for (; iter != splinePoints.end(); ++iter) {
            double t = stepSize*i;
            cv::Point2d point = std::pow(1-t,3)*splineStrip.p0 + std::pow(1-t,2)*t*3*splineStrip.c1 +
                    std::pow(t,3)*splineStrip.p1 + std::pow(t,2)*(1-t)*3*splineStrip.c2;
            *iter = point;
            i += 1;
        }
        cv::polylines(dstImage,splinePoints, false, cv::Scalar(1,1,1));
    }
}
