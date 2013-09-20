#ifndef DENSE_KITCHEN_H
#define DENSE_KITCHEN_H

#include "Utilities/include.h"
#include "Network/Network.h"
#include "ImageProcessing/ImageProcessor.h"
#include "Statistics/Analytics.h"
#include "Configuration/ConfigurationManager.h"
#include "Debugging/Debug.h"

class DenseKitchen
{
public:
    DenseKitchen() {}
    ~DenseKitchen() {}
    void readConfig(std::string path);
    void start();

private:
    network::Network network;
    image_processing::ImageProcessor imageProcessing;
    statistics::Analytics statistics;
    configuration::ConfigurationManager  configuration;
    
    //debug::Debug debug;

    FrameList frames;

    bool isRunning;
};

#endif