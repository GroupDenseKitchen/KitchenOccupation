#include "DenseKitchen.hpp"

bool DenseKitchen::initialize(std::string path) {
    isInitialized = true;

    PROFILER_ITERATION_START();
    algorithmFactory.clear();
    /*
     *  REGISTER ALGORITHMS HERE
     */
    algorithmFactory.add("ImageProcessor",                   new image_processing::ImageProcessor());
    algorithmFactory.add("BackgroundModelMoG",               new image_processing::BackgroundModelMoG());
    algorithmFactory.add("ForegroundRegionExtractorDefault", new image_processing::ForegroundRegionExtractorDefault());
    algorithmFactory.add("TrackingBruteForce",               new image_processing::TrackingBruteForce());
    algorithmFactory.add("Analytics",                        new statistics::Analytics());

    if(!settings.readConfig(path)) {
        LOG("DenseKitchen initialization error", "settings file read error! path: \"" << path << "\"");
        isInitialized = false;
    }
    if(!network.initialize(settings)) {
        LOG("DenseKitchen initialize error", "Network module could not be initialized!");
        isInitialized = false;
    }
    if(!imageProcessor.populateSubAlgorithms(settings, "ImageProcessor", algorithmFactory)) {
        LOG("DenseKitchen initialize error", "Image processor failed when populating sub algorighms");
        isInitialized = false;
    }
    if(!statistics.populateSubAlgorithms(settings, "Statistics", algorithmFactory)) {
        LOG("DenseKitchen initialize error", "Satisitics failed when populating sub algorighms");
        isInitialized = false;
    }
    if(!imageProcessor.initialize(settings)) {
        LOG("DenseKitchen initialize error", "ImageProcessor could not be initialized!");
        isInitialized = false;
    }
    if(!statistics.initialize(settings)) {
        LOG("DenseKitchen initialize error", "Statisitics could not be initialized!");
        isInitialized = false;
    }

    if(!evaluation.initialize(settings, &frames)) {
        LOG("DenseKitchen initialize error", "Evaluation could not be initialized!");
        isInitialized = false;
    }
    PROFILER_ITERATION_END();

    return isInitialized;
}

bool DenseKitchen::singleIteration() {

    bool iterationSuccess = true;

    if(isInitialized) {
        PROFILER_ITERATION_START();
            PROFILER_START("Network deque");
            Frame* currentFrame = network.dequeFrame();
            std::cerr << currentFrame->cameras[0].getImage("rawImage").size() << "FrameSize\n";
            std::cerr << evaluation.frameList->size() << " It's bigger than yours! \n";
            PROFILER_END();
            if(currentFrame){
                frames.append(*currentFrame);
                delete currentFrame;
                imageProcessor.process(frames);
                statistics.process(frames);
                //evaluation.frameList = &frames;
                evaluation.currentFrame();
                evaluation.printStats();

            }else{
                iterationSuccess = false;
            }
        PROFILER_ITERATION_END();
    } else {
        LOG("DenseKitchen singleIteration error", "DenseKitchen not initialized properly, initialize!");
        iterationSuccess = false;
    }

    PROFILER_DUMP();

    return iterationSuccess;
}
