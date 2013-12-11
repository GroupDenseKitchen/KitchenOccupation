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
#include "ImageProcessing/CircleDetection.hpp"
#include "ImageProcessing/kinectSegmentation.hpp"
#include "ImageProcessing/StereoBlockMatching.hpp"
#include "Analytics/Analytics.hpp"
#include "Analytics/FlowEstimator.hpp"
#include "Analytics/QueDetector.hpp"
#include "Analytics/QueSeverityEstimator.hpp"

#include "Evaluation/Evaluation.hpp"
#include "Evaluation/EntryExitEvaluator.hpp"
#include "Evaluation/TrackerEvaluator.hpp"


/*!
 * \brief     Main program class.
 * \details   It provides the interface for people detection and counting engine.
 *            Run in order: Init() once, readConfig(...) once, singleIteration() as many times as wished.
 */
class DenseKitchen
{
public:

    /*!
     * \brief   Constructor.
     */
    DenseKitchen() { isInitialized = false; }

    /*!
     * \brief   Destructor.
     */
    ~DenseKitchen() {}

    /*!
     * \brief      Initialize the program using a specified configuration file.
     * \details    Loads system settings and configures all the different program modules.
     * \param path Path to the configuration file
     * \return     Returns false if any of its modules fail.
     */
    bool initialize(std::string path);

    /*!
     * \brief    Reset program completely.
     * \details  Clears all temporary system settings and variables.
     */
    void reset();

    /*!
     * \brief     Run one iteration of the program.
     * \details   Deque one frame, perform person tracking and update statistics.
     * \return    False if the program wants to terminate, otherwise True.
     */
    bool singleIteration();

    /*!
     * \brief getFrames
     * \return A pointer to the active FrameList.
     */
    FrameList* getFrames();

private:
    FrameList frames;

    bool isInitialized;
    bool isEvalInitialized;

    network::Network network;
    configuration::ConfigurationManager settings;

    AlgorithmFactory algorithmFactory;
    image_processing::ImageProcessor imageProcessor;
    statistics::Analytics analytics;
    evaluation::Evaluation evaluator;

    std::string configPath;

};

#endif
