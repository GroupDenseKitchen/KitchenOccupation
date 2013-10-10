#include "DenseKitchen.hpp"

bool DenseKitchen::init(){

    bool networkSuccess = network.init(settings);
    return networkSuccess;
}

bool DenseKitchen::readConfig(std::string path) {
    
    configPath = path;
    return settings.readConfig(path);
}

bool DenseKitchen::singleIteration() {
    
    bool iterationSuccess = true;

    PROFILE_ITERATION_START();
        PROFILE_START("Network deque");
            Frame* currentFrame = network.dequeFrame();
        PROFILE_END();
        if(currentFrame){
            frames.append(*currentFrame);
            delete currentFrame;
            PROFILE_START("Image Processing");
                imageProcessing.process(frames);
            PROFILE_END();
            PROFILE_START("Statistics Processing");
                statistics.process(frames);
            PROFILE_END();
        }else{
            iterationSuccess = false;
        }
    PROFILE_ITERATION_END();

    return iterationSuccess;
}
