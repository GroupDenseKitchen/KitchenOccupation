#include "FrameList.hpp"

FrameList::FrameList(int framesToKeep) : framesToKeep(framesToKeep){
    frameCounter = 0;
    assert(framesToKeep >= 2 || framesToKeep == -1);
}

FrameList::~FrameList(){
    //TODO stub
}

Frame &FrameList::getCurrent(){
    return frames.back();
}

Frame &FrameList::getPrevious(){
    return frames[frames.size()-1];
}

bool FrameList::hasPrevious()
{
    return frames.size() > 1;
}

void FrameList::append(Frame f){

    if(frames.size() >= framesToKeep && framesToKeep != -1){
        frames.pop_front();
    }
    frames.push_back(f);
}
