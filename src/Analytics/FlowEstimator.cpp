#include "FlowEstimator.hpp"


namespace statistics
{
FlowEstimator::FlowEstimator() {

}

FlowEstimator::~FlowEstimator() {

}

bool FlowEstimator::initialize(configuration::ConfigurationManager& settings) {
    isInitialized = true;

    return isInitialized;
}

void FlowEstimator::process(FrameList &frames)
{
    if(frames.hasPrevious())
    {

        for(int n = 0; n < frames.getCurrent().getCameras().size(); n++)
        {
           CameraObject & currCam = frames.getCurrent().getCameras()[n];
           CameraObject & prevCam = frames.getPrevious().getCameras()[n];
           double enteredDiff = currCam.getEntered()-prevCam.getEntered();
           double exitedDiff = currCam.getExited()-prevCam.getExited();

           if(enteredDiff != 0)
           {
               inFlow.push_back(flowVectorPair(enteredDiff,frames.getFrameCount()));
           }
           if(exitedDiff != 0)
           {
               outFlow.push_back(flowVectorPair(exitedDiff,frames.getFrameCount()));
           }

           double  inFlowPerFrame  = 0;
           double  outFlowPerFrame = 0;
           double  totalIn = 0;
           double  totalOut = 0;

           for(int i = 0; i < inFlow.size(); i++)
           {
              totalIn = totalIn + inFlow[i].flow;
              inFlowPerFrame = totalIn/(inFlow[i].frameCount);
           }

           for(int j = 0; j < outFlow.size(); j++)
           {
               totalOut = totalOut + outFlow[j].flow;
               outFlowPerFrame = totalOut/(outFlow[j].frameCount);
           }


           // Debug writes flow of people that enters/exits into raw image
           cv::Mat raw = currCam.getImage("rawImage");
           std::string text = "";
           std::string text2 = "";
           int fontFace = cv::FONT_HERSHEY_PLAIN;
           double fontScale = 1;
           int thickness = 1;
           cv::Point2d pos1 = {10,55};
           cv::Point2d pos2 = {10,75};
           text = "InFlow: " + std::to_string(inFlowPerFrame);
           putText(raw, text, pos1, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
           text2 = "OutFlow: " + std::to_string(outFlowPerFrame);
           putText(raw, text2, pos2, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
         }
    }
}

flowVectorPair::flowVectorPair(int _flow ,unsigned int _frameCount)
{
    flow =_flow;
    frameCount = _frameCount;
}

}
