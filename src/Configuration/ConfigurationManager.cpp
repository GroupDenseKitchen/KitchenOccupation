#include "ConfigurationManager.hpp"

namespace configuration
{
    ConfigurationManager::ConfigurationManager() {}

    ConfigurationManager::~ConfigurationManager() {}

    void ConfigurationManager::readItem(std::string itemName, bool item)
    {
        if (configFile[itemName].type() == cv::FileNode::INT) {
            configFile[itemName] >> item;
            std::cout << "FDSHGJDJGH" << std::endl;
        } else {
            std::cout << "Dä bläv fäl" << std::endl;
            errors++;
        }
        /*
            LOG("Config error", "Error reading " << itemName << " from configuration file");
        }
        catch (cv::Exception& e) {
            std::cout << "Lol i caught you you nasty exception" << std::endl;
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            std::cout << debugging::logObject.getLastEntry() << std::endl;
            errors++;
        }*/

    }

    bool ConfigurationManager::readConfig(std::string filePath)
    {
        errors = 0;
        try {
            configFile.open(filePath,cv::FileStorage::READ);
        }
        catch (cv::Exception& herp) {
            LOG("Config error", "Error reading configuration file " << filePath);
            return false;
        }

        readItem("isDebug", isDebug);
        readItem("isTesting", isTesting);
        readItem("asdf", isDebug);

        LOG("Config", "Configuration file " << filePath << " read successfully with " << errors << " errors");
        std::cout << debugging::logObject.getLastEntry() << std::endl;
        return true;

    }

}
