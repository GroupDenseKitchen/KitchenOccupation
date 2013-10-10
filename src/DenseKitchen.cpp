#include "DenseKitchen.hpp"

bool DenseKitchen::init(){

    bool networkSuccess = network.init(settings);
    return networkSuccess;
}

bool DenseKitchen::readConfig(std::string path){
    
    configPath = path;
    return settings.readConfig(path);
}

bool DenseKitchen::singleIteration(){
    
    bool iterationSuccess = true;

    Frame* currentFrame = network.dequeFrame();
    if(currentFrame){

        frames.append(*currentFrame);
        delete currentFrame;
        imageProcessing.process(frames);
        statistics.process(frames);
    }else{
        iterationSuccess = false;
    }

    return iterationSuccess;
}
