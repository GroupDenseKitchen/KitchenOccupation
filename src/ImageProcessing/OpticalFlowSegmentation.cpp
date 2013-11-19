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
    if(frames.hasPrevious()){
        Frame current = frames.getCurrent();
        Frame previous = frames.getPrevious();
        computeOpticalFlow(current,previous);
    }
}

bool OpticalFlowSegmentation::initialize(configuration::ConfigurationManager &settings){

    maxPointsToTrack = 3000;
    minPointsToTrack = 500;

    int nFeatures = maxPointsToTrack;
    float scaleFactor = 1.2;
    int nLevels = 1; //default 8
    int edgeThreshold = 121;
    int firstLevel = 0;
    int WTA_K = 2;
    int scoreType =cv::ORB::HARRIS_SCORE;
    int patchSize = 121;

    detector = new cv::ORB(nFeatures,scaleFactor,nLevels,edgeThreshold,firstLevel,WTA_K,scoreType,patchSize);
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



void OpticalFlowSegmentation::computeOpticalFlow(Frame current, Frame previous){

    int numFrames = 3;
    static int counter = -1;
    static cv::Mat lastImage;

    for(unsigned int i=0; i < current.getCameras().size(); ++i){

            if(counter == -1){
                lastImage = previous.getCameras()[i].getImage("reallyRawImage").clone();
                imageToDrawOn = lastImage.clone();
            }

            if(counter == numFrames){

                CameraObject* currentCamera = &current.getCameras()[i];

                cv::Mat currentImage; //grayscale image
                cv::Mat prevImage;

                cv::cvtColor(currentCamera->getImage("reallyRawImage"), currentImage, CV_BGR2GRAY);
                cv::cvtColor(lastImage, prevImage, CV_BGR2GRAY);

                imageToDrawOn = lastImage.clone();

                getOpticalFlow(currentImage,prevImage);

                cv::Mat flowVectorImage = currentCamera->getImage("reallyRawImage").clone();
                lastImage = current.getCameras()[i].getImage("reallyRawImage").clone();


                counter = 0;
        }
    }
    counter = counter + 1;
}

void OpticalFlowSegmentation::removeOutliers(std::vector<FlowVector> FlowVectors){
    for(FlowVector& fv: FlowVectors){
        printf("%d %d \n",fv.pos.x,fv.pos.y);
    }
    exit(0);
}

void OpticalFlowSegmentation::getOpticalFlow(cv::Mat current, cv::Mat prev){

    std::vector<cv::Point2f> trackedPoints = std::vector<cv::Point2f>();
    std::vector<FlowVector> flowVectors = std::vector<FlowVector>();

    if(lastTrackedPoints.size() < minPointsToTrack){
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
        LOG("OpticalFLowSegmentation","ran detector");
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
