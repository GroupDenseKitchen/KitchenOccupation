#include "DenseKitchen.hpp"

bool DenseKitchen::init() {
    PROFILER_ITERATION_START();
    bool networkSuccess = network.initialize(settings);
    bool imageProcessingSuccess = imageProcessing.initialize(settings);
    bool statisticsSuccess = statistics.initialize(settings);
    bool evaluationSuccess = evaluation.initialize(settings, &frames);
    // LOG this...
    std::cout << networkSuccess << "\n";
    std::cout << imageProcessingSuccess << "\n";
    std::cout << statisticsSuccess << "\n";
    std::cout << evaluationSuccess << "\n";
    PROFILER_ITERATION_END();

    if(networkSuccess == false){
        std::cout << "Network init failed, is the video file path correct?" << std::endl;
        exit(-1);
    }

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
            std::cerr << "Here be evaluation code\n";
            evaluation.frameList = &frames;
            evaluation.currentFrame();
            std::cerr << evaluation.printStats();
        }else{
            iterationSuccess = false;
        }
    PROFILER_ITERATION_END();

    return iterationSuccess;
}
