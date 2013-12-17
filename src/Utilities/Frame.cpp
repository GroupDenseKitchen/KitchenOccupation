#include "Frame.hpp"

Frame::Frame(){
    currentQueStatus = -1;
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

void Frame::initRoomPopulations(std::vector<CameraObject> &_cameras){
    for(unsigned int n = 0; n < _cameras.size(); n++){
        std::string &currID = _cameras[n].getRoomID();
        if(populations.empty() || !hasID(currID)){
            roomPopulation curr;
            curr.people = 0;
            curr.roomID = currID;
            populations.push_back(curr);
        }
    }
}


bool Frame::hasID(std::string &_currID){
    for(unsigned int n = 0; n < populations.size(); n++){
        if(populations[n].roomID.compare(_currID) == 0){
            return true;
        }
    }
    return false;
}

void Frame::setPopulationInRoomID(int _newVal,std::string &_currID){
    for(unsigned int n = 0; n < populations.size(); n++){
        if(populations[n].roomID.compare(_currID) == 0){
            populations[n].people = _newVal;
        }
    }
}

int Frame::getPopulationInRoomID(std::string &_currID){
    for(unsigned int n = 0; n < populations.size(); n++){
        if(populations[n].roomID.compare(_currID) == 0){
            return populations[n].people;
        }
    }
    return -1;
}

void Frame::setQueStatus(int newQueStatus) {
    currentQueStatus = newQueStatus;
    //qDebug() << "QUEUE STATUS: " << currentQueStatus;
}

int Frame::getQueStatus() {
    return currentQueStatus;
}
