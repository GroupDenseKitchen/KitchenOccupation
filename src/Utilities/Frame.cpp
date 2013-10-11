#include "Frame.hpp"

CameraObject::CameraObject()
{

}

CameraObject::~CameraObject()
{

}

void CameraObject::addImage(std::string tag, cv::Mat image)
{
    processHistory.push_back(ProcessHistory(tag, "TODO: time...", image));
    images[tag] = image;
}

bool CameraObject::hasImage(std::string tag)
{
    return images.find(tag) != images.end();
}

cv::Mat CameraObject::getImage(std::string tag)
{
    if(!hasImage(tag))
        LOG("Frame CameraObject Error", "No image tagged \"" << tag << "\" exist!");
    return images[tag];
}


Frame::Frame(){

}

Frame::~Frame(){
    //TODO stub
}

void Frame::addCamera(CameraObject c){
    cameras.push_back(c);
}


std::vector<CameraObject> &Frame::getCameras(){
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

    for(std::vector<CameraObject>::iterator camera = cameras.begin(); camera != cameras.end(); ++camera) {
        if(roomID.compare(camera->roomID)){
            roomImages.push_back(camera->getImage("raw"));
        }
    }
    return roomImages;
}
