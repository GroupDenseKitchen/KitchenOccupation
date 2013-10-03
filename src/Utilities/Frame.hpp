#ifndef FRAME_H
#define FRAME_H
#include <opencv2/core/core.hpp>

#include <map>

class Frame{
public:
	Frame(cv::Mat image);
	~Frame();
	
    void addHistory(std::string tag, cv::Mat image);

private:
    cv::Mat baseImage;

    std::map<std::string,cv::Mat> processHistory;
};

#endif
