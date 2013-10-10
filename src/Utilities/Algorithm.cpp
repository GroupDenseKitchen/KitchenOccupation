#include "Algorithm.hpp"

bool Algorithm::initialize(configuration::ConfigurationManager & configuration)
{
    for(int n = 0; n < algorithms.size(); n++)
    {
        PROFILE_START(algorithmTag[n]+" initialization");
            algorithms[n].initialize(configuration);
        PROFILE_END();
    }
}

void Algorithm::process(FrameList & frames)
{
    for(int n = 0; n < algorithms.size(); n++)
    {
        PROFILE_START(algorithmTag[n]);
            algorithms[n].process(frames);
        PROFILE_END();
    }
}

void Algorithm::addAlgorithm(std::string tag, Algorithm algorithm)
{
    algorithmTag.push_back(tag);
    algorithms.push_back(algorithm);
}

void Algorithm::clearAlgorithms()
{
    algorithms.clear();
    algorithmTag.clear();
}
