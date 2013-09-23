#ifndef FRAME_H
#define FRAME_H
#include <opencv2/core/core.hpp>


class Frame{
public:
	Frame(cv::Mat image);
	~Frame();
	
private:
    cv::Mat image;

};

#endif