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
    std::vector<std::string> hereBeStrings, hereMightBeStrings;
    bool success = settings.readConfig("testConf.yml");

    hereMightBeStrings = settings.getStringSeq("Walla walla stringSeq");
    debugging::logObject.dumpToConsole();

    std::cerr << "Third element is: " << hereMightBeStrings[3] << std::endl;


    //return settings.readConfig(path);
    settings.setString("Walla walla string", "derp");
    settings.setInt("Walla walla int", 1337);
    settings.setBool("Walla walla bool", true);

    hereBeStrings.push_back("string0");
    hereBeStrings.push_back("string1");
    hereBeStrings.push_back("string2");
    hereBeStrings.push_back("string3");

    settings.setStringSeq("Walla walla stringSeq", hereBeStrings);
    settings.writeToFile("testConf.yml");
    std::cerr << "dafuq?\n";
    hereMightBeStrings = settings.getStringSeq("Walla walla stringSeq");
    debugging::logObject.dumpToConsole();
    return success;

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
