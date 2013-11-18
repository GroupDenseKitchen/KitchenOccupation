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

    if(frames.hasPrevious()){

        Frame current = frames.getCurrent();
        Frame previous = frames.getPrevious();
        for(unsigned int i=0; i < current.getCameras().size(); ++i){
            CameraObject* currentCamera = &current.getCameras()[i];
            CameraObject* previousCamera = &previous.getCameras()[i];

            cv::Mat currentImage;
            cv::Mat prevImage;
            cv::cvtColor(currentCamera->getImage("reallyRawImage"), currentImage, CV_BGR2GRAY);
            cv::cvtColor(previousCamera->getImage("reallyRawImage"), prevImage, CV_BGR2GRAY);

            getOpticalFlow(currentImage,prevImage);
            cv::Mat flowVectorImage = currentCamera->getImage("reallyRawImage").clone();

            paintFlowVectors(flowVectorImage,lastFlowVectors);

            currentCamera->addImage("flowVectors",flowVectorImage);
        }
    }
}

bool OpticalFlowSegmentation::initialize(configuration::ConfigurationManager &settings)
{
    detector = new cv::ORB();
    isInitialized = true;
    return isInitialized;
}

void OpticalFlowSegmentation::paintFlowVectors(cv::Mat image, std::vector<FlowVector> flowVectors){

    for(FlowVector& fl : flowVectors){
        cv::line(image,fl.pos,fl.pos+fl.flow,cv::Scalar(255,255,0),1);
    }

    cv::namedWindow("w1n");
    cv:imshow("w1n",image);
}

void OpticalFlowSegmentation::getOpticalFlow(cv::Mat current, cv::Mat prev){

    std::vector<cv::Point2f> trackedPoints = std::vector<cv::Point2f>();
    std::vector<FlowVector> flowVectors = std::vector<FlowVector>();

    if(lastTrackedPoints.size() < 500){
        std::vector<cv::KeyPoint> currentKeyPoints = std::vector<cv::KeyPoint>();
        detector->detect(prev,currentKeyPoints);
        trackedPoints.clear();
        trackedPoints = std::vector<cv::Point2f>(currentKeyPoints.size());
        for(int i=0; i < currentKeyPoints.size(); ++i){
            trackedPoints[i] = currentKeyPoints[i].pt;
            initialTrackCoordinates[i] = currentKeyPoints[i].pt;
        }
        qDebug() << "found " << trackedPoints.size() << " new points";
    }else{
        trackedPoints = lastTrackedPoints;
    }

    std::vector<uchar> status = std::vector<uchar>();
    std::vector<float> error = std::vector<float>();

    cv::Size windowSize = cv::Size(31,31);
    int maxLevel = 3;
    cv::TermCriteria terminate = cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30,0.01);
    int flags = 0;

    std::vector<cv::Point2f> nextPoints;

    //get optical flow
    cv::calcOpticalFlowPyrLK(prev,current,trackedPoints,nextPoints,status,error,windowSize,maxLevel,terminate,flags);

    //only remember the points we managed to track
    lastTrackedPoints.clear();
    flowVectors.clear();
    for(int i=0; i < status.size(); ++i){
        if(status[i] == 1){
            lastTrackedPoints.push_back(trackedPoints[i]);
        }else{

        }
    }

    lastFlowVectors = flowVectors;
}

}
