#include "DenseKitchen.h"

void DenseKitchen::readConfig(std::string path)
{
    configuration.readConfig(path);
}

/*
  OBS: Qt creator (projektets officiella utvecklingsmiljö) stödjer olika färgkodning
       för variabler som är medlemsvariabler respektive icke medlemsvariabler (som standard).
       Därav är prefix/postfix för medlemsvariabler onödiga för oss i vårt arbeta och bör slopas?
 */
void DenseKitchen::start()
{
    debug.start(this);
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
