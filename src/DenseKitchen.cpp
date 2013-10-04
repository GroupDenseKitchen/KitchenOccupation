#include "DenseKitchen.hpp"

void DenseKitchen::readConfig(std::string path)
{
    configuration.readConfig(path);
}

void DenseKitchen::start(){

    isRunning = false;
    
    debug.start(this);

    network.start();
    while(isRunning){
        
        if(network.hasNewFrame()){
            Frame currentFrame = network.dequeFrame();
            frames.append(currentFrame);

            imageProcessing.process(frames);
            statistics.process(frames);
        }
        debug.process();
    }
}