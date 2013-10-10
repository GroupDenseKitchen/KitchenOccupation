#ifndef FRAME_H
#define FRAME_H
#include <opencv2/core/core.hpp>

#include <map>

typedef struct CameraObject
{
    std::string roomID;
    cv::Mat rawImage;

    //debug information
    std::map<std::string,cv::Mat> processHistory;
} CameraObject;


class Frame
{
public:
    Frame();
    ~Frame();

    void addCamera(CameraObject c);

    std::vector<CameraObject> getCameras();
    std::vector<cv::Mat> getRoomImages(std::string roomID);
    std::vector<std::string> getRoomIDs();

private:
    std::vector<CameraObject> cameras;
};

#endif // FRAME_H
