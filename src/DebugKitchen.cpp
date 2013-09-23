#include "DebugKitchen.h"

void DebugKitchen::readConfig(std::string path)
{
    configuration.readConfig(path);
}

void DebugKitchen::start()
{
    //debug.start(this);
    network.start();
    while(isRunning)
    {
        if(network.hasNewFrame())
        {
            Frame currentFrame = network.dequeFrame();
            frames.append(currentFrame);

            imageProcessing.processFrame(frames);
            statistics.processFrame(frames);
        }
    }
}
