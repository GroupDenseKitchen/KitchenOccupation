#ifndef FRAME_LIST_H
#define FRAME_LIST_H

#include "utilities.hpp"
#include "Frame.hpp"

class FrameList{
public:
	FrameList();
	~FrameList();

	void append(Frame newFrame);
private:
	std::vector<Frame> frames;
};

#endif
