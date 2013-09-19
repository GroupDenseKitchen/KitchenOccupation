#ifndef DENSE_KITCHEN_H
#define DENSE_KITCHEN_H

#include "Utilities/include.h"
#include "NetworkModule/NetworkModule.h"
#include "ImageProcessingModule/ImageProcessingModule.h"
#include "StatisticsModule/StatisticsModule.h"
#include "ConfigurationModule/ConfigurationModule.h"
#include "DebugModule/DebugModule.h"

class DenseKitchen
{
public:
    DenseKitchen() {}
    ~DenseKitchen() {}
    void readConfig(std::string path);
    void run();

private:
    network::Network                  network;
    image_processing::ImageProcessing imageProcessing;
    statistics::Statistics            statistics;
    configuration::Configuration      configuration;
    debug::Debug                      debug;

    FrameList frames;
};

#endif