#ifndef DENSE_KITCHEN_H
#define DENSE_KITCHEN_H

#include "Utilities/utilities.hpp"
#include "Network/Network.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Debugging/Debug.hpp"
#include "Utilities/Algorithm.hpp"

#include <QtCore/QtCore>

#include "ImageProcessing/ImageProcessor.hpp"
#include "ImageProcessing/BackgroundModel.hpp"
#include "ImageProcessing/ForegroundRegionExtractor.hpp"
#include "ImageProcessing/Tracking.hpp"
#include "Analytics/Analytics.hpp"

/*!
 *  \brief     Main program class.
 *  \details   It provides the interface for people detection and counting engine.
 *             Run in order: Init() once, readConfig(...) once, singleIteration() as many times as wished.
 *  \version   0.1
 *  \date      2013-10-07
 */
class DenseKitchen
{
public:

    /*!
       \brief   Constructor.
    */
    DenseKitchen() { isInitialized = false; }

    /*!
       \brief   Destructor.
    */
    ~DenseKitchen() {}

    /*!
       \brief     Initialize the program using a config file specified.
       \return    False if it fails, otherwise True.
    */
    bool initialize(std::string path);

    /*!
       \brief     Run one iteration of the program.
       \details   Deque one frame and perform person tracking and update the statistics.
       \return    False if the program want to terminate, otherwise True.
    */
    bool singleIteration();

    FrameList frames;   //TODO: make private and provide interface (?)
private:
    bool isInitialized;

    network::Network network;
    configuration::ConfigurationManager settings;
    AlgorithmFactory algorithmFactory;
    image_processing::ImageProcessor imageProcessor;
    statistics::Analytics statistics;

    std::string configPath;
};

#endif
