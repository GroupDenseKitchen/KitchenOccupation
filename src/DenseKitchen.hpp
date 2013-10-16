#ifndef DENSE_KITCHEN_H
#define DENSE_KITCHEN_H

#include "Utilities/utilities.hpp"
#include "Network/Network.hpp"
#include "ImageProcessing/ImageProcessor.hpp"
#include "Analytics/Analytics.hpp"
#include "Analytics/Evaluation.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Debugging/Debug.hpp"

#include <QtCore/QtCore>

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
    DenseKitchen() {}

    /*!
       \brief   Destructor.
    */
    ~DenseKitchen() {}

    /*!
       \brief     Initialize the program.
       \return    False if it fails, otherwise True.
    */
    bool init();

    /*!
       \brief       Read a config file with program parameters.
       \param path  The config file and its path.
       \return      False if it fails, otherwise True.
    */
    bool readConfig(std::string path);

    /*!
       \brief     Run one iteration of the program.
       \details   Deque one frame and perform person tracking and update the statistics.
       \return    False if the program want to terminate, otherwise True.
    */
    bool singleIteration();
    FrameList frames;
private:
    //statistics::Evaluation asdf;
    network::Network network;
    image_processing::ImageProcessor imageProcessing;
    statistics::Analytics statistics;
    configuration::ConfigurationManager settings;
    evaluation::Evaluation evaluation;
    //FrameList frames;
    std::string configPath;

};

#endif
