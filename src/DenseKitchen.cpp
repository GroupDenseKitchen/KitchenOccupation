#include "DenseKitchen.h"

void DenseKitchen::readConfig(std::string path)
{
    configuration.readConfig(path);
}

/*
  OBS: Qt creator (projektets officiella utvecklingsmilj�) st�djer olika f�rgkodning
       f�r variabler som �r medlemsvariabler respektive icke medlemsvariabler (som standard).
       D�rav �r prefix/postfix f�r medlemsvariabler on�diga f�r oss i v�rt arbeta och b�r slopas?
 */
void DenseKitchen::run()
{
    debug.start(this);
    network.start();
    while(isRunning)
    {
        if(network.hasNewFrame())
        {
            frames.add(network.dequeFrame());
            imageProcessing.processFrame(frames);
            statistics.processFrame(frames);
        }
    }
}
