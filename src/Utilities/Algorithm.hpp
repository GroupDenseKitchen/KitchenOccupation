#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "utilities.hpp"
#include "FrameList.hpp"
#include "../Configuration/ConfigurationManager.hpp"

/*! 
 *  \brief     Base class for pipeline algorithms
 *  \details   Derive form this class to maintain a simple interface for all algorithms used in the processing pipeline.
 *  \version   1.0
 */
class Algorithm
{
public:
    virtual bool initialize(configuration::ConfigurationManager & configuration);
    virtual void process(FrameList & frames);

    void addAlgorithm(std::string tag, Algorithm algorithm);
    void clearAlgorithms();

protected:
    std::vector<Algorithm> algorithms;
    std::vector<std::string> algorithmTag;
};

#endif 
//ALGORITHM_HPP
