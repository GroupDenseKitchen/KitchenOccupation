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
       \brief   Constructor.
    */
    Algorithm();

    /*!
       \brief   Initializes all sub algorithms.
    */
    virtual bool initialize(configuration::ConfigurationManager & config);

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
    bool isInitialized;
    std::vector<Algorithm *> algorithms;
    std::vector<std::string> algorithmTag;
};

/*!
   \brief   Set the variable to config data if such exist, otherwise log error and fail initialization
*/
#define REQUIRE(c, variable, name)              if(!c.configure(name, variable)){LOG("Initialization error", name << " not specified"); isInitialized = false;}

/*!
   \brief   Set the variable to config data if such exist, otherwise log warning and set variable to default value
*/
#define CONFIG(c, variable, name, defaultValue) if(!c.configure(name, variable)){LOG("Initialization warning", "Property \"" << name << "\" not specified, uses default value: " << defaultValue); variable = defaultValue;}

#endif 
//ALGORITHM_HPP
