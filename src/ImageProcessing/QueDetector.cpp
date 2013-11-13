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
        CONFIG(settings, velocityScaleFactor, "velocityScaleFactor", 1)
        CONFIG(settings, splineLengthThreshold, "splineLengthThreshold", DBL_MAX); //This value is meaningless and needs to be determined in some way

        return isInitialized;
    }

    void QueDetector::process(FrameList &frames)
    {
        for (CameraObject & camera: frames.getCurrent().getCameras()) {
            std::vector<Object> objects = camera.getObjects();
            if(!camera.hasImage("rawImage"))
            {
                LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
                return;
            }
            cv::Mat debugImage = camera.getImage("rawImage").clone();
            if( objects.size() > 1) {


                //TODO:make sure center of mass and velocity is calculated somewhere else
                //Simulate center of mass and velocity
                int width = debugImage.cols;
                int height = debugImage.rows;
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator (seed);
                std::uniform_int_distribution<int> vDist(-100,100);
                for (Object & obj: objects) {
                    obj.centerOfMass = obj.center;
                    obj.velocity = cv::Point2d(vDist(generator), vDist(generator));
                }
                std::vector<Que> ques;
                detectQues( objects, ques );

                for (Que & que: ques) {
                    drawQue( debugImage,que );
                }
            }
            camera.addImage("Splines",debugImage);
        }

        /*
        std::vector<CameraObject> cameras = frames.getCurrent().getCameras();

        if(!cameras[0].hasImage("rawImage"))
        {
            LOG("ImageProcessing Error", "Image \"rawImage\" not set in current frame!");
            return;
        }
        cv::Mat debugImage = cameras[0].getImage("rawImage").clone();

        //TODO: remove this test code and get objects from cameras instead
        std::vector<Object> objects;
        {
        int numTestObjects = 10;
        int width = debugImage.cols;
        int height = debugImage.rows;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);
        std::uniform_int_distribution<int> xDist(0,width -1);
        std::uniform_int_distribution<int> yDist(0,height -1);
        std::uniform_int_distribution<int> vDist(-100,100);


        for (int i = 0; i < numTestObjects; ++i) {
            Object obj;
            obj.id = i;
            obj.centerOfMass = cv::Point2d(xDist(generator),yDist(generator));
            obj.velocity = cv::Point2d(vDist(generator), vDist(generator));
            objects.push_back(obj);
        }
        }

        std::vector<Que> ques;
        detectQues( objects, ques );

        for (Que que: ques) {
            drawQue( debugImage,que );
        }
        cameras[0].addImage("Splines",debugImage);

        //TODO:Figure out why addImage("Splines",image); doesn't work and replave viewing below
        cv::namedWindow("Spline debug window");
        cv::imshow("Spline debug window",debugImage);*/

    }

    void QueDetector::splineFromObjects( std::vector<Object> & objects , std::vector<SplineStrip>& spline, double maxSegmentLength )
    {
        spline.clear();

        Object previousObject;
        for(Object & nextObject: objects) {
            if ( nextObject.id != begin(objects)->id ) {
                SplineStrip strip(previousObject.centerOfMass,
                                  previousObject.centerOfMass + 3*velocityScaleFactor*previousObject.velocity,
                                  nextObject.centerOfMass - 3*velocityScaleFactor*nextObject.velocity,
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
        for (Object & obj: objects) {
            std::vector<Object> feasibleDestinations;
            extractFeasibleDestinations( obj, objects, feasibleDestinations );

            DirectedQueEdge queEdge( obj ); //Initialize to point to itself
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
        for (Object & obj: allObjects) {
            if ( obj.id != fromObject.id ) {
                destObjects.push_back( obj ); //TODO: Use better filtering for feasible destinations.
            }
        }
    }

    void QueDetector::findBestDestination(Object &fromObject, std::vector<Object> &destObjects, DirectedQueEdge &destEdge)
    {
        for (Object & destObj: destObjects) {
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
        //TODO: Do something smarter than (make sure each que is disjoint from all other ques)
        //Right now all edges/objects in que are put into one single que
        //Also, remove loops
        ques.clear();
        Que theQue;
        for (DirectedQueEdge & queEdge: queEdges) {
            for (SplineStrip & strip: queEdge.spline) {
                theQue.splineStrips.push_back( strip );
            }
            theQue.queEdges.push_back(queEdge);

            //Insert any objects into the que if they are either a source or destination
            //of a que edge.
            if ( theQue.queObjects.find(queEdge.from.id) == theQue.queObjects.end() ) {
                theQue.queObjects.insert( {queEdge.from.id, queEdge.from} );
            }
            if ( theQue.queObjects.find(queEdge.to.id) == theQue.queObjects.end() ) {
                theQue.queObjects.insert( {queEdge.to.id, queEdge.to} );
            }
        }
        ques.push_back( theQue );
    }

}
