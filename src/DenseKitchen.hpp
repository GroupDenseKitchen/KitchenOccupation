 #ifndef DENSE_KITCHEN_H
#define DENSE_KITCHEN_H

#include "Utilities/utilities.hpp"
#include "Network/Network.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Debugging/Debug.hpp"
#include "Utilities/Algorithm.hpp"

#include <QtCore/QtCore>

// Include algorithms here
#include "ImageProcessing/ImageProcessor.hpp"
#include "ImageProcessing/BackgroundModelMoG.hpp"
#include "ImageProcessing/ForegroundRegionExtractorDefault.hpp"
#include "ImageProcessing/TrackingBruteForce.hpp"
#include "ImageProcessing/EntryExitCounter.hpp"
#include "ImageProcessing/QueDetector.hpp"

#include "ImageProcessing/PersonDetection.hpp"

#include "Analytics/Analytics.hpp"
#include "Analytics/FlowEstimator.hpp"
#include "Analytics/Evaluation.hpp"


/*!
 *  \brief     Main program class.
 *  \details   It provides the interface for people detection and counting engine.
 *             Run in order: Init() once, readConfig(...) once, singleIteration() as many times as wished.
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
       \brief     Initialize the program using a settings file specified.
       \return    Return false if any of its modules fail.
    */
    bool initialize(std::string path);

    /*!
        \brief    Reset program completely.
    */
    void reset();

    /*!
       \brief     Run one iteration of the program.
       \details   Deque one frame and perform person tracking and update the statistics.
       \return    False if the program want to terminate, otherwise True.
    */
    bool singleIteration();

    FrameList frames;   //TODO: make private and provide interface (?)
private:

    bool isInitialized;
    bool isEvalInitialized;

    network::Network network;
    configuration::ConfigurationManager settings;
    evaluation::Evaluation evaluation;

    AlgorithmFactory algorithmFactory;
    image_processing::ImageProcessor imageProcessor;
    statistics::Analytics statistics;

    std::string configPath;

};

#endif
