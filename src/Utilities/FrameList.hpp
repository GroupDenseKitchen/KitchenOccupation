#ifndef FRAME_LIST_H
#define FRAME_LIST_H

#include "utilities.hpp"
#include "Frame.hpp"

#include <deque>

class FrameList{
public:
    FrameList(int framesToKeep = -1);
	~FrameList();

    Frame getCurrent();
	void append(Frame newFrame);
private:
    std::deque<Frame> frames;
    int framesToKeep;
    int frameCounter;
};

#endif
