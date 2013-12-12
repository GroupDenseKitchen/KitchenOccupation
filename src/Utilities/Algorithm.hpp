#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "utilities.hpp"
#include "FrameList.hpp"
#include "../Configuration/ConfigurationManager.hpp"

class AlgorithmFactory;

/*!
 * \brief     Base class for pipeline algorithms
 * \details   Derive form this class to maintain a simple interface for all algorithms used in the processing pipeline.
 * \version   1.0
 */
class Algorithm
{
public:

    /*!
     * \brief   Constructor.
     */
    Algorithm();

    /*!
     * \brief   Destructor.
     */
    virtual ~Algorithm() {}

    /*!
     * \brief   Initializes algorithm and all sub algorithms.
     * \param settings A ConfigurationManager object containing system settings.
     */
    virtual bool initialize(configuration::ConfigurationManager &settings);

    /*!
     * \brief   Adds sub algorithms from settings, then calls this function for every sub algorithm.
     * \details A sub algorithm is another Algorithm that is a part in this algorithms pipeline,
     *          meaning that it is initialized when this algorithm is initialized and it is executed
     *          when this algorithm is executed, both in the order specified in settings (the order they are stored in the algorithms vector).
     * \param   settings A ConfigurationManager object containing system settings.
     * \param   algorithmName The name of the algortihm to be added to the pipeline.
     * \param   algorithmFactory The current pipeline part to be populated.
     * \return  True if successful.
     */
    bool populateSubAlgorithms(configuration::ConfigurationManager &settings, std::string algorithmName, AlgorithmFactory &algorithmFactory);

    /*!
     * \brief   Processes all sub algorithms.
     * \details Calls the process function on all populated sub algorithms in the order specified by the configuration file.
     * \param   frames The current FrameList.
     */
    virtual void process(FrameList & frames);

    /*!
     * \brief   Remove all sub algorithms.
     */
    void clearAlgorithms();

protected:

    /*!
     * \brief       Is true if the algorithm is initialized.
     * \details     It should be set in the initialize method and may be modified by REQUIRE, if this macro is used. An algorithm which has isInitialized as false will not have its process method called.
     */
    bool isInitialized;

    /*!
     * \brief       A vector of sub algorithms.
     * \details     The sub algorithms will be initialized in order when initialize is called. They will also be processed in order when process is called, if process is not overriden.
     */
    std::vector<Algorithm *> algorithms;

    /*!
     * \brief       A vector of sub algorithms names, mapped in order to algorithms.
     */
    std::vector<std::string> algorithmTag;

private:
     void addAlgorithm(std::string tag, Algorithm * algorithm);
};

/*!
 * \brief   Set the variable to config data if such exist, otherwise log error and fail initialization
 */
#define REQUIRE(c, variable, name)              if(!c.configure(name, variable, 0)){LOG("Initialization error", name << " not specified"); isInitialized = false;}

/*!
 * \brief   Set the variable to config data if such exist, otherwise log warning and set variable to default value
 */
#define CONFIG(c, variable, name, defaultValue) if(!c.configure(name, variable, defaultValue)){LOG("Initialization warning", "Property \"" << name << "\" not specified, uses default value: " << defaultValue);}


#include "AlgorithmFactory.hpp"

#endif
//ALGORITHM_HPP
