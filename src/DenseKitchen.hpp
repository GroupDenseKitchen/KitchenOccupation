#ifndef DENSE_KITCHEN_H
#define DENSE_KITCHEN_H

#include "Utilities/utilities.hpp"
#include "Network/Network.hpp"
#include "ImageProcessing/ImageProcessor.hpp"
#include "Analytics/Analytics.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Debugging/Debug.hpp"

#include <QtCore/QtCore>

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
