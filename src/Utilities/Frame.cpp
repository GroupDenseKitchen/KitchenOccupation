#include "Frame.hpp"

Frame::Frame(){

}

void Frame::addHistory(std::string tag, cv::Mat image){
    processHistory.insert(std::pair<std::string,cv::Mat>(tag,image));
}

void Frame::addRawImage(cv::Mat image){
	rawImages.push_back(image);
}

Frame::~Frame(){
    //TODO stub
}
