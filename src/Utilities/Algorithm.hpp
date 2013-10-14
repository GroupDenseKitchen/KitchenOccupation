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
    /*!
       \brief   Initializes all sub algorithms.
    */
    virtual bool initialize(configuration::ConfigurationManager & configuration);

    /*!
       \brief   Processes all sub algorithms.
    */
    virtual void process(FrameList & frames);

    /*!
       \brief   Add sub algorithm.
    */
    void addAlgorithm(std::string tag, Algorithm * algorithm);


    /*!
       \brief   Remove all sub algorithms
    */
    void clearAlgorithms();

protected:
    std::vector<Algorithm *> algorithms;
    std::vector<std::string> algorithmTag;
};

#endif 
//ALGORITHM_HPP
