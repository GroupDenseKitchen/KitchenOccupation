#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "../Utilities/utilities.hpp"

namespace configuration{
    class ConfigurationManager{
    public:
        ConfigurationManager();
        ~ConfigurationManager();
        void readConfig(std::string filePath);
    };
}

#endif
