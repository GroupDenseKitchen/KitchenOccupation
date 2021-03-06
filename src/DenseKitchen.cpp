#include "DenseKitchen.hpp"

bool DenseKitchen::initialize(std::string path) {
    isInitialized = true;
    isEvalInitialized = true;
    counter = 0;
    PROFILER_ITERATION_START();
    algorithmFactory.clear();

    /*
     *  REGISTER ALGORITHMS HERE
     */
    algorithmFactory.add("ImageProcessor",                   new image_processing::ImageProcessor());
    algorithmFactory.add("BackgroundModelMoG",               new image_processing::BackgroundModelMoG());
    algorithmFactory.add("ForegroundRegionExtractorDefault", new image_processing::ForegroundRegionExtractorDefault());
    algorithmFactory.add("TrackingBruteForce",               new image_processing::TrackingBruteForce());
    algorithmFactory.add("EntryExitCounter",                 new image_processing::EntryExitCounter());
    algorithmFactory.add("StereoBlockMatching",              new image_processing::StereoBlockMatching());
    algorithmFactory.add("KinectSegmentation",               new image_processing::KinectSegmentation());
    algorithmFactory.add("Analytics",                        new statistics::Analytics());
    algorithmFactory.add("CircleDetection",                  new image_processing::CircleDetection());
    algorithmFactory.add("QueDetector",                      new statistics::QueDetector());
    algorithmFactory.add("FlowEstimator",                    new statistics::FlowEstimator());
    algorithmFactory.add("QueSeverityEstimator",             new statistics::QueSeverityEstimator());


    algorithmFactory.add("EntryExitEvaluator",               new evaluation::EntryExitEvaluator());
    //algorithmFactory.add("TrackerEvaluator",                 new evaluation::TrackerEvaluator());


    if(!settings.readConfig(path)) {
        LOG("DenseKitchen initialization error", "settings file read error! path: \"" << path << "\"");
        isInitialized = false;
    }
    printf("read config file \n");
    if(!network.initialize(settings)) {
        printf("error initing network! \n");
        LOG("DenseKitchen initialize error", "Network module could not be initialized!");
        isInitialized = false;
    }
    if(!imageProcessor.populateSubAlgorithms(settings, "ImageProcessor", algorithmFactory)) {
        printf("error populating image processor \n");
        LOG("DenseKitchen initialize error", "Image processor failed when populating sub algorighms");
        isInitialized = false;
    }
    if(!analytics.populateSubAlgorithms(settings, "Analytics", algorithmFactory)) {
        printf("error populating analytics \n");
        LOG("DenseKitchen initialize error", "Satisitics failed when populating sub algorighms");
        isInitialized = false;
    }
    if(!evaluator.populateSubAlgorithms(settings, "Evaluation", algorithmFactory)) {
        printf("error populating  evaluation \n");
        LOG("DenseKitchen initialize error", "Satisitics failed when populating sub algorighms");
        isEvalInitialized = false;
    }
    if(!imageProcessor.initialize(settings)) {
        printf("error initing image processor \n");
        LOG("DenseKitchen initialize error", "ImageProcessor could not be initialized!");
        isInitialized = false;
    }
    if(!analytics.initialize(settings)) {
        printf("error initing analytics \n");
        LOG("DenseKitchen initialize error", "statistics could not be initialized!");
        isInitialized = false;
    }
    if(!evaluator.initialize(settings)) {
	printf("error initing evaluation \n");
        LOG("DenseKitchen initialize error", "Evaluator could not be initialized!");
         isEvalInitialized = false;
    }
    PROFILER_ITERATION_END();

    if (isInitialized)
        settings.writeToFile(); // Save successful initialization to config

    return isInitialized;
}

void DenseKitchen::reset()
{
    network.reset();
    imageProcessor.reset();
    analytics.reset();

    debugging::logObject.clear();

    // Reset global variables

}

bool DenseKitchen::singleIteration() {

    bool iterationSuccess = true;
    counter++;
    if(isInitialized) {
        PROFILER_ITERATION_START();
            PROFILER_START("Network deque");
            Frame* currentFrame = network.dequeFrame();
            PROFILER_END();
            if(currentFrame){
                frames.append(*currentFrame);
                delete currentFrame;

                PROFILER_START("Image Processing");
                imageProcessor.process(frames);
                PROFILER_END();

                PROFILER_START("Statistics Processing");
                analytics.process(frames);
                PROFILER_END();

                if (isEvalInitialized) {
                    PROFILER_START("Evaluation")
                    evaluator.process(frames);
                    PROFILER_END();
                    //evaluator.printToLog(); // Prints MOTA & MOTP for every frame.
                }

                network.broadcastData(frames.getCurrent());

            } else {
                if (isEvalInitialized) {
                    evaluator.printToLog();
                    iterationSuccess = false;
                }
            }
        PROFILER_ITERATION_END();
    } else {
        LOG("DenseKitchen singleIteration error", "DenseKitchen not initialized properly, initialize!");
        iterationSuccess = false;
    }

    return iterationSuccess;
}

FrameList* DenseKitchen::getFrames()
{
    return &frames;
}

configuration::ConfigurationManager *DenseKitchen::getSettings()
{
    return &settings;
}


