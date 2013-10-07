#ifndef FRAME_LIST_H
#define FRAME_LIST_H

#include "utilities.hpp"
#include "Frame.hpp"

#include <deque>

/*!
 *  \brief     A container of cronoligicaly ordered Frames.
 *  \details   Keeps infinitly or a configurable number of Frames as history apart from the current Frame.
 *  \version   0.1
 *  \date      2013-10-07
 */
class FrameList
{
public:
    FrameList(int framesToKeep = -1);
	~FrameList();

    /*!
       \brief   Get the current (latest) Frame.
    */
    Frame getCurrent();

    /*!
       \brief   Append the FrameList with the latest Frame.
    */
	void append(Frame newFrame);
private:
    std::deque<Frame> frames;
    int framesToKeep;
    int frameCounter;
};

#endif
