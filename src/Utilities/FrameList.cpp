#include "FrameList.hpp"

FrameList::FrameList(int framesToKeep) : framesToKeep(framesToKeep){
    frameCounter = 0;
    assert(framesToKeep >= -1);
}

FrameList::~FrameList(){
    //TODO stub
}

Frame FrameList::getCurrent(){
    return frames.back();
}

void FrameList::append(Frame f){

    if(frames.size() >= framesToKeep && framesToKeep != -1){
        frames.pop_front();
    }
    frames.push_back(f);
}