#include "QueDetector.hpp"

namespace image_processing
{
    QueDetector::QueDetector()
    {
    }

    bool QueDetector::initialize(configuration::ConfigurationManager &settings)
    {
        isInitialized = true;

        CONFIG(settings, maxSplineSegmentLength, "maxSplineSegmentLength", 3);
        CONFIG(settings, splineLengthThreshold, "splineLengthThreshold", 200); //This value is meaningless and needs to be determined in some way

        return isInitialized;
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
        std::vector<Object> objects;
        Object obj1,obj2,obj3;
        obj1 = Object(); obj1.id =1; obj1.centerOfMass = cv::Point2d(0,0); obj1.velocity = cv::Point2d(10,30); objects.push_back(obj1);
        obj2 = Object(); obj2.id =2; obj2.centerOfMass = cv::Point2d(100,50); obj2.velocity = cv::Point2d(80,10); objects.push_back(obj2);
        obj3 = Object(); obj2.id =3; obj3.centerOfMass = cv::Point2d(300,300); obj3.velocity = cv::Point2d(70,90); objects.push_back(obj3);

        splineFromObjects(objects, dumbQue.splineStrips, maxSplineSegmentLength );
        dumbQue.queObjects.insert( {obj1.id,obj1} );
        dumbQue.queObjects.insert( {obj2.id,obj2} );
        dumbQue.queObjects.insert( {obj3.id,obj3} );


        cv::Mat image = cameras[0].getImage("rawImage").clone();
        drawQue(image,dumbQue);
        cameras[0].addImage("Splines",image);

        //TODO:Figure out why addImage("Splines",image); doesn't work and replave viewing below
        cv::namedWindow("Spline debug window");
        cv::imshow("Spline debug window",image);

    }

    void QueDetector::splineFromObjects( std::vector<Object> & objects , std::vector<SplineStrip>& spline, double maxSegmentLength )
    {
        spline.clear();

        Object previousObject;
        for(Object nextObject: objects) {
            if ( nextObject.id != begin(objects)->id ) { //TODO: Fix this
                SplineStrip strip(previousObject.centerOfMass,
                                  previousObject.centerOfMass + 3*previousObject.velocity,
                                  nextObject.centerOfMass - 3*nextObject.velocity,
                                  nextObject.centerOfMass);
                subdivideSpline(strip, spline, maxSegmentLength);
            }
            previousObject = nextObject;
        }
    }

    void QueDetector::drawQue(cv::Mat &dstImage, const Que & que)
    {
        //OpenCV wants this in c-arrays (of arrays)
        const int numPoints = 4; //Cubic spline
        const int numStrips = que.splineStrips.size();
        cv::Point **points = new cv::Point*[numStrips];
        int numberOfPoints[numStrips];

        //Fill per-spline-arrays with points on each spline strip
        for (int j=0; j < numStrips;++j) {
            SplineStrip splineStrip = que.splineStrips[j];
            numberOfPoints[j] = numPoints;
            points[j] = new cv::Point[numPoints];
            points[j][0] = splineStrip.p0;
            points[j][1] = splineStrip.c1;
            points[j][2] = splineStrip.c2;
            points[j][3] = splineStrip.p1;
        }
        //Draw spline
        cv::polylines(dstImage,const_cast<const cv::Point**>(points),numberOfPoints,que.splineStrips.size(), false, cv::Scalar(255,255,255));

        //Clean up spline points
        for (int j = 0; j < numStrips; ++j) {
            delete[] points[j];
        }

        //Draw object center of masses and velocities
        auto iter = que.queObjects.begin();
        for (; iter != que.queObjects.end(); ++iter){
            cv::Point2d center = iter->second.centerOfMass;
            cv::circle( dstImage, center, 5 ,cv::Scalar(0,255,0), 2 );
            cv::line( dstImage,center, center + iter->second.velocity, cv::Scalar(255,0,0), 2 );
        }

    }

    void QueDetector::subdivideSpline(SplineStrip &strip, std::vector<SplineStrip> &spline, double maxSegmentLength)
    {
        //Calculate new Control points
        cv::Point2d helpPoint = (strip.c1 + strip.c2)*0.5;
        cv::Point2d leftC1 = (strip.p0 + strip.c1)*0.5;
        cv::Point2d leftC2 = (leftC1 + helpPoint)*0.5;
        cv::Point2d rightC2 = (strip.c2 + strip.p1)*0.5;
        cv::Point2d rightC1 = (helpPoint + rightC2)*0.5;
        cv::Point2d center = (leftC2 + rightC1)*0.5;

        SplineStrip left(strip.p0, leftC1, leftC2, center);
        SplineStrip right(center, rightC1, rightC2, strip.p1);

        //Contine subdividing if not good enough
        if ( left.maxSegmentLength() < maxSegmentLength ) {
            spline.push_back(left);
        } else {
            subdivideSpline(left, spline, maxSegmentLength);
        }

        if ( right.maxSegmentLength() < maxSegmentLength ) {
            spline.push_back(right);
        } else {
            subdivideSpline(right, spline, maxSegmentLength);
        }
    }

    double QueDetector::splineLength(std::vector<SplineStrip> spline) {
        double length = 0;
        for (SplineStrip strip: spline) {
            length += strip.length();
        }
        return length;
    }

    void QueDetector::detectQues(std::vector<Object> &objects, std::vector<Que> &ques)
    {
        //Find good pairs of objects in que (closest close-enough destination for each object)
        std::vector<DirectedQueEdge> edges;
        for (Object obj: objects) {
            std::vector<Object> feasibleDestinations;
            extractFeasibleDestinations( obj, objects, feasibleDestinations );

            DirectedQueEdge queEdge( obj );
            findBestDestination( obj, feasibleDestinations, queEdge );
            if ( obj.id != queEdge.to.id ) { //Good destination found
                edges.push_back( queEdge );
            }
        }

        //Create ques by stringing together connected edges
        quesFromEdges(edges, ques);
    }

    void QueDetector::extractFeasibleDestinations(Object &fromObject,
                                                  std::vector<Object> &allObjects,
                                                  std::vector<Object> &destObjects)
    {
        destObjects.clear();
        for (Object obj: allObjects) {
            if ( obj.id != fromObject.id ) {
                destObjects.push_back( obj ); //TODO: Use better filtering for feasible destinations.
            }
        }
    }

    void QueDetector::findBestDestination(Object &fromObject, std::vector<Object> &destObjects, DirectedQueEdge &destEdge)
    {
        for (Object destObj: destObjects) {
            std::vector<SplineStrip> tmpSpline;
            std::vector<Object> tmpPair( {fromObject, destObj} );
            splineFromObjects(tmpPair, tmpSpline, maxSplineSegmentLength ); //TODO: Use different constant
            double length = splineLength( tmpSpline );
            if ( length < splineLengthThreshold && length < destEdge.distance ) {
                destEdge.to = destObj;
                destEdge.spline = tmpSpline;
                destEdge.distance = length;
            }
        }
    }

    void QueDetector::quesFromEdges(std::vector<DirectedQueEdge> &queEdges, std::vector<Que> &ques)
    {
        //TODO: Do something smarter (make sure each que is disjoint from all other ques)
        ques.clear();
        Que theQue;
        for (DirectedQueEdge queEdge: queEdges) {
            for (SplineStrip strip: queEdge.spline) {
                theQue.splineStrips.push_back( strip );
            }

            theQue.queEdges.push_back(queEdge);
            if ( theQue.queObjects.find(queEdge.from.id) != theQue.queObjects.end() ) {
                theQue.queObjects.insert( {queEdge.from.id, queEdge.from} );
            }
            if ( theQue.queObjects.find(queEdge.to.id) != theQue.queObjects.end() ) {
                theQue.queObjects.insert( {queEdge.to.id, queEdge.to} );
            }
        }
    }

}
