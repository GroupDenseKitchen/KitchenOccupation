#include "DenseKitchen.hpp"

void DenseKitchen::readConfig(std::string path)
{
    configuration.readConfig(path);
}

void DenseKitchen::start()
{
    isRunning = false;

    #ifdef DEBUG
        debug.start(this);
    #endif

    network.start();
    while(true){
        while(isRunning)
        {
            if(network.hasNewFrame())
            {
                Frame currentFrame = network.dequeFrame();
                frames.append(currentFrame);

                imageProcessing.process(frames);
                statistics.process(frames);
            }
        #ifdef DEBUG
            running = false;
        #endif
        }
    }
}
