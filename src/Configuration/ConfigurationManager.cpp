#include "ConfigurationManager.hpp"

namespace configuration
{
    ConfigurationManager::ConfigurationManager() {}

    ConfigurationManager::~ConfigurationManager() {}

    bool ConfigurationManager::isDebugMode() {return isDebug;}
    bool ConfigurationManager::isTestingMode() {return isTesting;}
    bool ConfigurationManager::isGroundTruthAvailable() {return hasGroundTruth;}

    int ConfigurationManager::getNumberOfCameras() {return nCameras;}

    std::string ConfigurationManager::getVideoFilePath() {return videoFilePath;}
    std::string ConfigurationManager::getGroundTruthPath() {return groundTruthPath;}

    void ConfigurationManager::readItem(std::string itemName, bool& item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::INT) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }

    void ConfigurationManager::readItem(std::string itemName, int& item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::INT) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }


    void ConfigurationManager::readItem(std::string itemName, float& item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::FLOAT) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }

    void ConfigurationManager::readItem(std::string itemName, std::string& item)
    {
        nItemsRead++;
        if (configFile[itemName].type() == cv::FileNode::STR) {
            configFile[itemName] >> item;
        } else {
            LOG("Config error", "Error reading " << itemName << " from configuration file");
            nErrors++;
        }
    }

    void ConfigurationManager::readItem(std::string itemName, cv::Mat& item)
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
        std::cout << "Well well, congratulations on coming this far \n";
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

        cv::FileNode nodes;
        try {
            nodes = configFile.root();
        }
        catch (cv::Exception& e) {
            LOG("Config error", "Error reading configuration file " << filePath);
            return false;
        }

        int nodeType = cv::FileNode::NONE;
        for(cv::FileNodeIterator node = nodes.begin(); node != nodes.end(); node++)
        {
            /*
            try {
                node = configFile[i];
            }
            catch (cv::Exception& e) {
                nodeType = cv::FileNode::NONE;
            }*/

            switch (nodeType) {
                case cv::FileNode::INT:
                    readItem("isDebug", isDebug);
                    readItem("isTesting", isTesting);
                    readItem("hasGroundTruth", hasGroundTruth);
                    readItem("nCameras", nCameras);
                    break;
                case cv::FileNode::STRING:
                    readItem("videoFilePath", videoFilePath);
                    readItem("groundTruthPath", groundTruthPath);
                    break;
                default:
                    std::cout << "Something went wrong \n";
            }
        }


        /*
        readItem("isDebug", isDebug);
        readItem("isTesting", isTesting);
        readItem("hasGroundTruth", hasGroundTruth);
        readItem("nCameras", nCameras);
        readItem("videoFilePath", videoFilePath);
        readItem("groundTruthPath", groundTruthPath);
*/
        LOG("Config", "Configuration file " << filePath << " read successfully with " <<
                      nErrors << " errors out of a total " << nItemsRead << " attempts.");
        nErrors = 0;
        nItemsRead = 0;

        return true;

    }

}
