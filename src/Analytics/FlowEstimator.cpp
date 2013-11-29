#include "FlowEstimator.hpp"


namespace statistics
{
FlowEstimator::FlowEstimator() {

}

FlowEstimator::~FlowEstimator() {

}

bool FlowEstimator::initialize(configuration::ConfigurationManager& settings) {
    isInitialized = true;
    isSetup = false;
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

            if(isSetup == false)
            {
                //if first time --> create inflow/outflow vectors for camera
                CameraFlow camFlow;
                camFlow.inFlow.push_back(flowVectorPair(enteredDiff,frames.getFrameCount()));
                camFlow.outFlow.push_back(flowVectorPair(exitedDiff,frames.getFrameCount()));
                cameraFlowVector.push_back(camFlow);
            }

            if(exitedDiff != 0 && isSetup)
            {
                //Did someone exit? If so notice this by inserting nr of poeple etered in specific frame
                //at camera n.
                cameraFlowVector[n].outFlow.push_back(flowVectorPair(exitedDiff,frames.getFrameCount()));
            }

            if(enteredDiff != 0 && isSetup)
            {
                //Same as obove but if someone entered.
                cameraFlowVector[n].inFlow.push_back(flowVectorPair(enteredDiff,frames.getFrameCount()));
            }

            //local variables
            double  inFlowPerFrame  = 0;
            double  outFlowPerFrame = 0;
            double  totalIn = 0;
            double  totalOut = 0;



            //Calculation of the "flow", now given in people/frame
            for(int i = 0; i < cameraFlowVector[n].inFlow.size(); i++)
            {
                totalIn = totalIn + cameraFlowVector[n].inFlow[i].flow;
                inFlowPerFrame = totalIn/(cameraFlowVector[n].inFlow[i].frameCount);
            }

            for(int j = 0; j < cameraFlowVector[n].outFlow.size(); j++)
            {
                totalOut = totalOut + cameraFlowVector[n].outFlow[j].flow;
                outFlowPerFrame = totalOut/(cameraFlowVector[n].outFlow[j].frameCount);
            }
        }
        isSetup = true;
    }
}

flowVectorPair::flowVectorPair(int _flow ,unsigned int _frameCount)
{
    flow =_flow;
    frameCount = _frameCount;
}
}
