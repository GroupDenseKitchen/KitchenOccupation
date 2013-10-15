#include "Algorithm.hpp"

Algorithm::Algorithm()
{
    isInitialized = false;
}

bool Algorithm::initialize(configuration::ConfigurationManager & config)
{
    bool success = true;

    // Initialize sub algorithms
    for(int n = 0; n < algorithms.size(); n++)
    {
        PROFILER_START(algorithmTag[n]+" initialization");
        if(!algorithms[n]->initialize(config)) {
            success = false;
            LOG(algorithmTag[n]+" initialization error", algorithmTag[n] << " initialization failed!");
        } else {
            algorithms[n]->isInitialized = true;
        }
        PROFILER_END();
    }
    return success;
}

bool Algorithm::initialize(configuration::ConfigurationManager &config, std::string AlgorithmName)
{
    //TODO: add sub patterns from config
    return initialize(config);
}

void Algorithm::process(FrameList & frames)
{
    for(int n = 0; n < algorithms.size(); n++)
    {
        if(algorithms[n]->isInitialized)
        {
            PROFILER_START(algorithmTag[n]);
            algorithms[n]->process(frames);
            PROFILER_END();
        }
        else
            LOG(algorithmTag[n]+" process error", algorithmTag[n] << " must be initialized befor it is executed!");
    }
}

void Algorithm::addAlgorithm(std::string tag, Algorithm * algorithm)
{
    algorithmTag.push_back(tag);
    algorithms.push_back(algorithm);
}

void Algorithm::clearAlgorithms()
{
    algorithms.clear();
    algorithmTag.clear();
}
