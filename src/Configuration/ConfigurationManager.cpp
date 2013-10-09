#include "ConfigurationManager.hpp"

namespace configuration
{
    ConfigurationManager::ConfigurationManager() {}

    ConfigurationManager::~ConfigurationManager() {}

    void ConfigurationManager::readItem(std::string itemName, bool item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::INT) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }

    void ConfigurationManager::readItem(std::string itemName, int item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::INT) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }


    void ConfigurationManager::readItem(std::string itemName, float item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::FLOAT) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }

    void ConfigurationManager::readItem(std::string itemName, std::string item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::STRING) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }

    void ConfigurationManager::readItem(std::string itemName, cv::Mat item)
    {
        nItemsRead++;
        try {
            configFile[itemName] >> item;
        }
        catch (cv::Exception& e) {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
            return;
        }
    }

    bool ConfigurationManager::readConfig(std::string filePath)
    {
        nErrors = 0;
        nItemsRead = 0;
        try {
            configFile.open(filePath,cv::FileStorage::READ);
        }
        catch (cv::Exception& e) {
            LOG("Config error", "Error reading configuration file " << filePath);
            return false;
        }
        cv::Mat fgsfds;

        readItem("isDebug", isDebug);
        readItem("isTesting", isTesting);
        readItem("hasGroundTruth", hasGroundTruth);
        readItem("nCameras", nCameras);
        readItem("videoFilePath", videoFilePath);
        readItem("groundTruthPath", groundTruthPath);


        LOG("Config", "Configuration file " << filePath << " read successfully with " <<
                      nErrors << " errors out of a total " << nItemsRead << " attempts.");
        nErrors = 0;
        nItemsRead = 0;

        return true;

    }

}
