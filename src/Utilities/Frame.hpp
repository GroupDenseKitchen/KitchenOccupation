#ifndef FRAME_H
#define FRAME_H
#include <opencv2/core/core.hpp>

#include <map>

class Frame{
public:
	Frame();
	~Frame();
	
	void addRawImage(cv::Mat image);
    void addHistory(std::string tag, cv::Mat image);

    friend class MainDebugWindow;
private:
    std::vector<cv::Mat> rawImages;

    std::map<std::string,cv::Mat> processHistory;
};

#endif
