#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "../Utilities/include.h"

namespace configuration{
    class ConfigurationManager{
    public:
        ConfigurationManager();
        ~ConfigurationManager();
        void readConfig(std::string filePath);
    };
}

#endif