#include "Algorithm.hpp"

Algorithm::Algorithm() {
    isInitialized = false;
}

bool Algorithm::initialize(configuration::ConfigurationManager & settings) {
    isInitialized = true;

    // Initialize sub algorithms
    for(unsigned int n = 0; n < algorithms.size(); n++) {
        PROFILER_START(algorithmTag[n]+" initialization");
        if(!algorithms[n]->initialize(settings)) {
            isInitialized = false;
            LOG(algorithmTag[n]+" initialization error", algorithmTag[n] << " initialization failed!");
        } else {
            algorithms[n]->isInitialized = true;
        }
        PROFILER_END();
    }
    return isInitialized;
}

bool Algorithm::populateSubAlgorithms(configuration::ConfigurationManager& settings,
                                      std::string algorithmName,
                                      AlgorithmFactory &algorithmFactory) {
    bool isSuccessful = true;
    if(settings.hasStringSeq(algorithmName)) {
        std::vector<std::string> algorithmNames = settings.getStringSeq("ImageProcessor");
        std::vector<std::string>::iterator algorithmName = algorithmNames.begin();
        for(; algorithmName != algorithmNames.end(); algorithmName++) {
            if(algorithmFactory.has(*algorithmName)) {
                addAlgorithm(*algorithmName, algorithmFactory.get(*algorithmName));
            } else {
                /* If the expected algorithm is no actual algorithm (that is registerd in the factory atleast),
                 * then it might be an alias for another list of algorithms. If this is the case (the if below),
                 * then a dummy algorithm is created to hold this algorithm list, and added as a sub-algorithm. */
                if(settings.hasStringSeq(*algorithmName)) {
                    addAlgorithm(*algorithmName+" [empty]", new Algorithm());
                } else {
                    LOG("ImageProcessor initialize error", "Algorithm \"" << *algorithmName << "\" not found! Unregistered or misspelled?");
                    isSuccessful = false;
                }
            }
        }
        // Let all sub algorithms add their sub algorithms
        for(unsigned int n = 0; n < algorithms.size(); n++) {
            isSuccessful &= algorithms[n]->populateSubAlgorithms(settings, algorithmTag[n], algorithmFactory);
        }
    }
    return isSuccessful;
}

void Algorithm::process(FrameList & frames) {
    for(unsigned int n = 0; n < algorithms.size(); n++) {
        if(algorithms[n]->isInitialized) {
            PROFILER_START(algorithmTag[n]);
            algorithms[n]->process(frames);
            PROFILER_END();
        } else {
            LOG(algorithmTag[n]+" process error", algorithmTag[n] << " must be initialized befor it is executed!");
        }
    }
}

void Algorithm::addAlgorithm(std::string tag, Algorithm * algorithm) {
    algorithmTag.push_back(tag);
    algorithms.push_back(algorithm);
}

void Algorithm::clearAlgorithms() {
    algorithms.clear();
    algorithmTag.clear();
}
