#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "../Utilities/utilities.hpp"

/*!
 *  \brief     Configuration provide config file functionality
 */
namespace configuration
{
/*!
 *  \brief     The ConfigurationManager manages configurations from file.
 *  \version   0.1
 *  \date      2013-10-07
 */
class ConfigurationManager
{
public:
    /*!
       \brief   Constructor.
    */
    ConfigurationManager();

    /*!
       \brief   Destructor.
    */
    ~ConfigurationManager();

    /*!
       \brief   Read a configuration file.
    */
    bool readConfig(std::string filePath);
};
}

#endif
