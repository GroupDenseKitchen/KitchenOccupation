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

bool Algorithm::initializeRoot(configuration::ConfigurationManager &config, std::string algorithmName, AlgorithmFactory &algorithmFactory)
{
    if(config.hasStringSeq(algorithmName))
    {
        std::vector<std::string> algorithms = config.getStringSeq("ImageProcessor");
        for(std::vector<std::string>::iterator algorithm = algorithms.begin(); algorithm != algorithms.end(); algorithm++)
        {
            if(algorithmFactory.has(*algorithm))
                addAlgorithm(*algorithm, algorithmFactory.get(*algorithm));
            else    //TODO: Add support to recursivly add sub-algorithms...
                LOG("ImageProcessor initialize error", "Algorithm \"" << *algorithm << "\" not found!");
        }
    }
    debugging::logObject.dumpToConsole();
    return initialize(config);
}


/*
std::vector<std::string> algorithms = config.getStringSequence("ImageProcessor");
for(std::vector<std::string>::iterator algorithm = algorithms.begin(); algorithm != algorithms.end(); algorithm++)
    if(AlgorithmFactory::has(*algorithm))
        addAlgorithm(*algorithm, AlgorithmFactory::get(*algorithm));
    else    //TODO: Add support to recursivly add sub-algorithms...
        LOG("ImageProcessor initialize error", "Algorithm \"" << *algorithm << "\" not found!");
*/

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
