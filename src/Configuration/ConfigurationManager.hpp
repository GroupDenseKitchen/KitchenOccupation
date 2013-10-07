#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "../Utilities/utilities.hpp"

/*!
 *  \brief     Configuration management
 *  \details   Provide config file functionality
 */
namespace configuration
{
/*!
 *  \brief
 *  \details
 *  \version   0.1
 *  \date      2013-10-07
 */
class ConfigurationManager
{
public:
    ConfigurationManager();
    ~ConfigurationManager();
    bool readConfig(std::string filePath);
};
}

#endif
