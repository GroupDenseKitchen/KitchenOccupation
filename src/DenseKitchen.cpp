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
    imageProcessor.populateSubAlgorithms(settings, "ImageProcessor", algorithmFactory);
    statistics.populateSubAlgorithms(settings, "Statistics", algorithmFactory);
    imageProcessor.initialize(settings);
    statistics.initialize(settings);
    PROFILER_ITERATION_END();

    return isInitialized;
}

bool DenseKitchen::singleIteration() {

    bool iterationSuccess = true;

    if(isInitialized) {
        PROFILER_ITERATION_START();
            PROFILER_START("Network deque");
            Frame* currentFrame = network.dequeFrame();
            PROFILER_END();
            if(currentFrame){
                frames.append(*currentFrame);
                delete currentFrame;
                imageProcessor.process(frames);
                statistics.process(frames);
            }else{
                iterationSuccess = false;
            }
        PROFILER_ITERATION_END();
    } else {
        LOG("DenseKitchen singleIteration error", "DenseKitchen not initialized properly, initialize!");
        iterationSuccess = false;
    }

    return iterationSuccess;
}
