#ifndef DEBUG_KITCHEN_H
#define DEBUG_KITCHEN_H

#include "Utilities/include.h"
#include "Network/Network.h"
#include "ImageProcessing/ImageProcessor.h"
#include "Analytics/Analytics.h"
#include "Configuration/ConfigurationManager.h"
#include "Debugging/Debug.h"

class DebugKitchen
{
public:
    DebugKitchen() {}
    ~DebugKitchen() {}
    void readConfig(std::string path);
    void start();

private:
    network::Network network;
    image_processing::ImageProcessor imageProcessing;
    statistics::Analytics statistics;
    configuration::ConfigurationManager  configuration;

    FrameList frames;

    bool isRunning;
};

#endif