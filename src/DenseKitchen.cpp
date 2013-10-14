#include "DenseKitchen.hpp"

bool DenseKitchen::init() {
    PROFILER_ITERATION_START();
    bool networkSuccess = network.initialize(settings);
    bool imageProcessingSuccess = imageProcessing.initialize(settings);
    bool statisticsSuccess = statistics.initialize(settings);
    // LOG this...
    std::cout << networkSuccess << "\n";
    std::cout << imageProcessingSuccess << "\n";
    std::cout << statisticsSuccess << "\n";
    PROFILER_ITERATION_END();
    return networkSuccess && imageProcessingSuccess && statisticsSuccess;
}

bool DenseKitchen::readConfig(std::string path) {
    
    configPath = path;
    return settings.readConfig(path);
}

bool DenseKitchen::singleIteration() {
    
    bool iterationSuccess = true;

    PROFILER_ITERATION_START();
        PROFILER_START("Network deque");
        Frame* currentFrame = network.dequeFrame();
        PROFILER_END();
        if(currentFrame){
            frames.append(*currentFrame);
            delete currentFrame;
            imageProcessing.process(frames);
            statistics.process(frames);
        }else{
            iterationSuccess = false;
        }
    PROFILER_ITERATION_END();

    return iterationSuccess;
}
