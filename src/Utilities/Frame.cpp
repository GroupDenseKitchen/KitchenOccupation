#include "Frame.hpp"

Frame::Frame(){

}

Frame::~Frame(){
    //TODO stub
}

void Frame::addCamera(CameraObject c){
    cameras.push_back(c);
}


std::vector<CameraObject> Frame::getCameras(){
    return cameras;
}


//TODO nontrivialish, verify it works...
std::vector<std::string> Frame::getRoomIDs(){

	std::vector<std::string> roomIDs;

	for(std::vector<CameraObject>::iterator it = cameras.begin(); it != cameras.end(); ++it) {

		if(std::find(roomIDs.begin(), roomIDs.end(), it->roomID) != roomIDs.end()){
			roomIDs.push_back(it->roomID);
		}
	}

	return roomIDs;
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
