#include "Frame.hpp"

Frame::Frame(cv::Mat image) : baseImage(image){

}

void Frame::addHistory(std::string tag, cv::Mat image){
    processHistory.insert(std::pair<std::string,cv::Mat>(tag,image));
}

Frame::~Frame(){
    //TODO stub
}
