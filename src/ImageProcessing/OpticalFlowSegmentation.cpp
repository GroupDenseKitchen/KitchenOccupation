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
    minPointsToTrack = 1000;

    int nFeatures = maxPointsToTrack;
    float scaleFactor = 1.2;
    int nLevels = 1; //default 8
    int edgeThreshold = 31;
    int firstLevel = 0;
    int WTA_K = 2;
    int scoreType =cv::ORB::HARRIS_SCORE;
    int patchSize = 31;

    detector = new cv::ORB(nFeatures,scaleFactor,nLevels,edgeThreshold,firstLevel,WTA_K,scoreType,patchSize);
    isInitialized = true;
    return isInitialized;
}

float OpticalFlowSegmentation::arcCosToXaxis(cv::Point2f _vector)
{
    cv::Point2f xAxis = cv::Point2f(1,0);
    float n = sqrt(_vector.dot(_vector));
    _vector.x = _vector.x /n;//normalize
    _vector.y = _vector.y / n;
    if(_vector.y > 0){
        return acos(_vector.dot(xAxis));
    }
    else{
        return 2*3.1415-acos(_vector.dot(xAxis));
    }
}


void OpticalFlowSegmentation::paintFlowVectors(cv::Mat imageToDrawOn, std::vector<FlowVector> flowVectors){

    int bb = 0;
    for(FlowVector& fl: flowVectors){
        cv::Point2f diff = fl.flow;
        float a = arcCosToXaxis(diff);
        float r = a * 255 / (2*3.1415) ;
        float g = 255 - a*255/(2*3.1415) ;
        int rr = (int)ceil(r);
        int gg = (int)ceil(g);

        cv::line(imageToDrawOn,fl.pos,fl.pos+fl.flow,cv::Scalar(rr,gg,0),1);
        bb = bb + 1;
        if(bb == 255){
            bb = 0;
        }
    }
}

void OpticalFlowSegmentation::computeOpticalFlow(Frame current, Frame previous){

    int numFrames = 2;
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

std::vector<FlowVector> OpticalFlowSegmentation::filterSmallest(std::vector<FlowVector> FlowVectors){
    std::vector<FlowVector> removedOutliers = std::vector<FlowVector>();
    for(FlowVector& fv: FlowVectors){
        float size = fv.flow.dot(fv.flow);
        if(size > 100 && size < 4000){
            removedOutliers.push_back(fv);
        }
    }
    return removedOutliers;
}

std::vector<FlowVector> OpticalFlowSegmentation::averageFlow(std::vector<FlowVector> flowVectors, cv::Size imageSize){
    std::vector<FlowVector> averagedFlow = std::vector<FlowVector>();
    std::vector<FlowBlock> blockData = std::vector<FlowBlock>();

    int blocksPerRow = 80;

    int blockWidth = imageSize.width / blocksPerRow;
    int blockHeight = imageSize.height / blocksPerRow;

    int hBlocks = imageSize.width / blockWidth;
    int wBlocks = imageSize.height / blockHeight;

    std::vector<std::vector<FlowVector> > blocks = std::vector<std::vector<FlowVector> >(hBlocks*wBlocks);
    for(int i=0; i < hBlocks*wBlocks; ++i){
        blocks[i] = std::vector<FlowVector>();
    }

    for(FlowVector& fl : flowVectors){
        int blockIndex = (int)std::floor(fl.pos.x / blockWidth) + std::floor(fl.pos.y / blockHeight) * blocksPerRow;
        blocks[blockIndex].push_back(fl);
    }

    for(std::vector<FlowVector>& blockVectors : blocks){
        if(blockVectors.size() > 0){
            FlowVector averageVector;
            averageVector.pos = cv::Point2f(0,0);
            averageVector.flow = cv::Point2f(0,0);
            for(FlowVector& fl : blockVectors){
                averageVector.pos = averageVector.pos + fl.pos;
                averageVector.flow = averageVector.flow + fl.flow;
            }
            int numVectorsInBlock = blockVectors.size();
            averageVector.pos.x / numVectorsInBlock;
            averageVector.flow.x / numVectorsInBlock;
            averageVector.pos.y / numVectorsInBlock;
            averageVector.flow.y / numVectorsInBlock;
            averagedFlow.push_back(averageVector);
        }
    }

    return averagedFlow;
}

void OpticalFlowSegmentation::getOpticalFlow(cv::Mat current, cv::Mat prev){

    std::vector<cv::Point2f> trackedPoints = std::vector<cv::Point2f>();
    std::vector<FlowVector> flowVectors = std::vector<FlowVector>();

    if(lastTrackedPoints.size() < minPointsToTrack){
        std::vector<cv::KeyPoint> currentKeyPoints = std::vector<cv::KeyPoint>();
        detector->detect(prev,currentKeyPoints);
        trackedPoints = std::vector<cv::Point2f>(currentKeyPoints.size());

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
    int flags = 0; //cv::OPTFLOW_LK_GET_MIN_EIGENVALS;

    std::vector<cv::Point2f> nextPoints;

    //get optical flow
    cv::calcOpticalFlowPyrLK(prev,current,trackedPoints,nextPoints,status,error,windowSize,maxLevel,terminate,flags);

    //only remember the points we managed to track
    lastTrackedPoints.clear();
    flowVectors.clear();

    for(int i=0; i < status.size(); ++i){
        if(status[i] == 1){
            lastTrackedPoints.push_back(nextPoints[i]);
            FlowVector fl;
            fl.pos = keyframeCoordinates[i];
            fl.flow = keyframeCoordinates[i] - nextPoints[i];
            flowVectors.push_back(fl);
        }else{
            //lost a point
        }
    }

    flowVectors = filterSmallest(flowVectors);
    flowVectors = averageFlow(flowVectors,current.size());
    flowVectors = filterSmallest(flowVectors);

    qDebug() << "number of vectors! " << flowVectors.size();

    paintFlowVectors(imageToDrawOn,flowVectors);

    cv::namedWindow("w2n");
    cv:imshow("w2n",imageToDrawOn);
}

}
