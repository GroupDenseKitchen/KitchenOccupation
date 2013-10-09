#include "Frame.hpp"

Frame::Frame(){

}

void Frame::addHistory(std::string tag, cv::Mat image){
    processHistory.insert(std::pair<std::string,cv::Mat>(tag,image));
}

Frame::~Frame(){
    //TODO stub
}

void Frame::addCamera(CameraObject c){
	cameras.push_back(c);
}

std::vector<cv::Mat> Frame::getRoomImages(std::string roomID){

	std::vector<cv::Mat> roomImages;

	for(std::vector<CameraObject>::iterator it = cameras.begin(); it != cameras.end(); ++it) {
    	if(roomID.compare(it->roomID)){
    		roomImages.push_back(it->rawImage);
    	}
	}
	return roomImages;
}