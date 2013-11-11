#include "Frame.hpp"

Frame::Frame(){

}

Frame::~Frame(){

}

void Frame::addCamera(CameraObject c){
    cameras.push_back(c);
}


std::vector<CameraObject> &Frame::getCameras(){
    return cameras;
}

std::vector<std::string> Frame::getRoomIDs(){

    std::vector<std::string> roomIDs;

    for(std::vector<CameraObject>::iterator it = cameras.begin(); it != cameras.end(); ++it) {

        if(std::find(roomIDs.begin(), roomIDs.end(), it->getRoomID()) != roomIDs.end()){
            roomIDs.push_back(it->getRoomID());
        }
    }

    return roomIDs;
}

double Frame::getMomentaryFps() const
{
    return momentaryFps;
}

void Frame::setMomentaryFps(double value)
{
    momentaryFps = value;
}


std::vector<cv::Mat> Frame::getRoomImages(std::string roomID){
    
    std::vector<cv::Mat> roomImages;
    
    for(std::vector<CameraObject>::iterator camera = cameras.begin(); camera != cameras.end(); ++camera) {
        if(roomID.compare(camera->getRoomID())){
            roomImages.push_back(camera->getImage("raw"));
        }
    }
    return roomImages;
}
