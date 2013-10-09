#ifndef FRAME_H
#define FRAME_H
#include <opencv2/core/core.hpp>

#include <map>

typedef struct CameraObject{
	std::string roomID;
	std::vector<CameraObject*> overlap;
	cv::Mat rawImage;
}CameraObject;


class Frame{
public:
	Frame();
	~Frame();
	
    void addHistory(std::string tag, cv::Mat image);
    void addCamera(CameraObject c);

    std::vector<cv::Mat> getRoomImages(std::string roomID);
    std::vector<std::string> getRoomIDs();

private:
    std::vector<CameraObject> cameras;

    std::map<std::string,cv::Mat> processHistory;
};

#endif
