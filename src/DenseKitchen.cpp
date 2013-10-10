#include "DenseKitchen.hpp"

bool DenseKitchen::init(){

    //TODO init stuff...
    return true;
}

bool DenseKitchen::readConfig(std::string path) {
    
    configPath = path;
    return settings.readConfig(path);
}

bool DenseKitchen::singleIteration() {
    
    bool iterationSuccess = true;

    if(network.hasNewFrame()){
        Frame currentFrame = network.dequeFrame();
        frames.append(currentFrame);

        imageProcessing.process(frames);
        statistics.process(frames);
    }else{
        iterationSuccess = false;
    }

    return iterationSuccess;
}
