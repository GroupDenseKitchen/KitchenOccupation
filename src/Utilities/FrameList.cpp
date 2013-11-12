#include "FrameList.hpp"

FrameList::FrameList(int framesToKeep) : framesToKeep(framesToKeep){
    frameCounter = 0;
    assert(framesToKeep >= 2 || framesToKeep == -1);

    //doorMask.zeros(640,480,CV_8UC3);
    //exclusionMask.zeros(640,480,CV_8UC3);
}

FrameList::~FrameList(){
    //TODO stub
}

Frame &FrameList::getCurrent(){
    return frames.back();
}

Frame &FrameList::getPrevious(){
    return frames[frames.size()-2];
}

bool FrameList::hasPrevious()
{
    return frames.size() > 1;
}

void FrameList::append(Frame f)
{

    if(frames.size() >= framesToKeep && framesToKeep != -1){
        frames.pop_front();
    }
    frames.push_back(f);
    frameCounter++;

    // smoothFps update
    if(f.getMomentaryFps() == 0) { // if first frame
        smoothFps = 0;
    } else if (smoothFps == 0) {   // if second frame
        smoothFps = f.getMomentaryFps();
    } else {
        smoothFps = smoothFps*0.9 + f.getMomentaryFps()*0.1;
    }
}

cv::Mat FrameList::getExclusionMask() const
{
    return exclusionMask;
}

void FrameList::setExclusionMask(const cv::Mat &value)
{
    exclusionMask = value.clone();
}

bool FrameList::hasExclusionMask()
{
    return !exclusionMask.empty();
}
cv::Mat FrameList::getDoorMask() const
{
    return doorMask;
}

void FrameList::setDoorMask(const cv::Mat &value)
{
    doorMask = value.clone();
}

bool FrameList::hasDoorMask()
{
    return !doorMask.empty();
}
cv::Mat FrameList::getInclusionMask() const
{
    return inclusionMask;
}

void FrameList::setInclusionMask(const cv::Mat &value)
{
    inclusionMask = value.clone();
}

bool FrameList::hasInclusionMask()
{
    return !inclusionMask.empty();
}

