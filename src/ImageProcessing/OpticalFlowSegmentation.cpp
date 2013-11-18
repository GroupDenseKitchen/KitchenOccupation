#include "OpticalFlowSegmentation.hpp"

namespace image_processing{

OpticalFlowSegmentation::OpticalFlowSegmentation()
{
}

OpticalFlowSegmentation::~OpticalFlowSegmentation()
{

}

void OpticalFlowSegmentation::process(FrameList &frames)
{

    static int counter = 0;
    static cv::Mat lastFrame;
    int numFrames = 3;

    if(frames.hasPrevious()){

        Frame current = frames.getCurrent();
        Frame previous = frames.getPrevious();

        for(unsigned int i=0; i < current.getCameras().size(); ++i){

            if(counter <= numFrames){
                lastFrame = frames.getPrevious().getCameras()[i].getImage("reallyRawImage").clone();
                imageToDrawOn = lastFrame.clone();
            }

            if(counter % numFrames == 0){

            CameraObject* currentCamera = &current.getCameras()[i];

            //lastFrame = frames.getPrevious().getCameras()[i].getImage("reallyRawImage");
            //CameraObject* previousCamera = &previous.getCameras()[i];

            cv::Mat currentImage; //grayscale image
            cv::Mat prevImage;

            cv::cvtColor(currentCamera->getImage("reallyRawImage"), currentImage, CV_BGR2GRAY);
            cv::cvtColor(lastFrame, prevImage, CV_BGR2GRAY);


            getOpticalFlow(currentImage,prevImage);
            cv::Mat flowVectorImage = currentCamera->getImage("reallyRawImage").clone();
            //paintFlowVectors(flowVectorImage,lastFlowVectors);

            //currentCamera->addImage("flowVectors",flowVectorImage);
            lastFrame = frames.getCurrent().getCameras()[i].getImage("reallyRawImage").clone();
            imageToDrawOn = lastFrame.clone();
        }
        }
    }

    counter = counter + 1;
}

bool OpticalFlowSegmentation::initialize(configuration::ConfigurationManager &settings)
{
    int nFeatures = 3000;
    float scaleFactor = 1.2;
    int nLevels = 2; //default 8
    int edgeThreshold = 31;
    int firstLevel = 0;

    detector = new cv::ORB(nFeatures,scaleFactor,nLevels,edgeThreshold,firstLevel);
    isInitialized = true;
    return isInitialized;
}

void OpticalFlowSegmentation::paintFlowVectors(cv::Mat image, std::vector<FlowVector> flowVectors){


    qDebug() << "got: " << flowVectors.size();
    for(FlowVector& fl : flowVectors){
        cv::line(image,fl.pos,fl.pos+fl.flow,cv::Scalar(255,255,0),1);
    }

    cv::namedWindow("w1n");
    cv:imshow("w1n",image);
}

void OpticalFlowSegmentation::getOpticalFlow(cv::Mat current, cv::Mat prev){

    std::vector<cv::Point2f> trackedPoints = std::vector<cv::Point2f>();
    std::vector<FlowVector> flowVectors = std::vector<FlowVector>();

    if(lastTrackedPoints.size() < 3000){
        std::vector<cv::KeyPoint> currentKeyPoints = std::vector<cv::KeyPoint>();
        detector->detect(prev,currentKeyPoints);
        trackedPoints = std::vector<cv::Point2f>(currentKeyPoints.size());
        flowVectors.clear();

        keyframeCoordinates.clear();
        keyframeCoordinates = std::vector<cv::Point2f>(trackedPoints.size());

        for(int i=0; i < currentKeyPoints.size(); ++i){
            trackedPoints[i] = currentKeyPoints[i].pt;
            keyframeCoordinates[i] = currentKeyPoints[i].pt;
        }
        qDebug() << "found " << trackedPoints.size() << " new points";
    }else{
        trackedPoints = lastTrackedPoints;
    }

    std::vector<uchar> status = std::vector<uchar>();
    std::vector<float> error = std::vector<float>();

    cv::Size windowSize = cv::Size(31,31);
    int maxLevel = 1;
    cv::TermCriteria terminate = cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30,0.01);
    int flags = 0;

    std::vector<cv::Point2f> nextPoints;

    //get optical flow
    cv::calcOpticalFlowPyrLK(prev,current,trackedPoints,nextPoints,status,error,windowSize,maxLevel,terminate,flags);

    //only remember the points we managed to track
    lastTrackedPoints.clear();



    for(int i=0; i < status.size(); ++i){
        if(status[i] == 1){
            lastTrackedPoints.push_back(nextPoints[i]);
            FlowVector fl;
            fl.pos = keyframeCoordinates[i];
            fl.flow = keyframeCoordinates[i] - trackedPoints[i];
            flowVectors.push_back(fl);
            cv::Point2f diff = nextPoints[i] - trackedPoints[i];
            if(diff.dot(diff) > 2 && diff.dot(diff) < 5000){
                /*
                cv::Point2f xAxis = cv::Point2f(1,0);
                cv::Point2f nDiff;
                nDiff.x = diff.x / sqrt(diff.dot(diff));
                nDiff.y = diff.y / sqrt(diff.dot(diff));

                float a = acos(nDiff.dot(xAxis));
                float r = a * 255 / (3.1415) ;
                float g = 255 - a*255/3.1415 ;
                int rr = (int)ceil(r);
                int gg = (int)ceil(g);
                printf("a is %f",a);
                printf("rr is: %d, gg is %d \n",rr,gg);
                */

                cv::line(imageToDrawOn,trackedPoints[i],nextPoints[i],cv::Scalar(64,181,34),1);
            }
        }else{
            //lost a point
        }
    }

    cv::namedWindow("w2n");
    cv:imshow("w2n",imageToDrawOn);
}

}
