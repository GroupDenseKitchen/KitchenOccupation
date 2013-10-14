#include "ConfigurationManager.hpp"

namespace configuration
{
    ConfigurationManager::ConfigurationManager() {}

    ConfigurationManager::~ConfigurationManager() {}

    bool ConfigurationManager::hasBool(std::string name)
    {
        bool isBool = false;
        bool exists = intBoolMap.find(name) != intBoolMap.end();
        if (exists) {
             isBool = intBoolMap[name] == 0 || intBoolMap[name] == 1;
        }
        if(!isBool) {
            LOG("Config Warning", "Boolean named: " << name << " unavailable.");
        }
        return isBool;
    }
    bool ConfigurationManager::hasInt(std::string name)
    {
        bool exists = intBoolMap.find(name) != intBoolMap.end();
        if (!exists) {
            LOG("Config Warning", "Int named: " << name << " unavailable.");
        }
        return exists;
    }
    bool ConfigurationManager::hasDouble(std::string name)
    {
        bool exists = doubleMap.find(name) != doubleMap.end();
        if (!exists) {
            LOG("Config Warning", "Double named: " << name << " unavailable.");
        }
        return exists;
    }

    bool ConfigurationManager::hasString(std::string name)
    {
        bool exists = stringMap.find(name) != stringMap.end();
        if (!exists) {
            LOG("Config Warning", "String named: " << name << " unavailable.");
        }
        return exists;
    }

    bool ConfigurationManager::getBool(std::string name)
    {
        if (hasBool(name)) {
            return intBoolMap[name];
        } else {
            LOG("Config Error", "Seriously?! Variable not found, use the \"hasBool\" function first");
            return false;
        }
    }

    int ConfigurationManager::getInt(std::string name)
    {
        if (hasInt(name)) {
            return intBoolMap[name];
        } else {
            LOG("Config Error", "Seriously?! Variable not found, use the \"hasInt\" function first");
            return 0;
        }
    }

    double ConfigurationManager::getDouble(std::string name)
    {
        if (hasDouble(name)) {
            return doubleMap[name];
        } else {
            LOG("Config Error", "Seriously?! Variable not found, use the \"hasDouble\" function first");
            return 0;
        }
    }

    std::string ConfigurationManager::getString(std::string name)
    {
        if (hasString(name)) {
            return stringMap[name];
        } else {
            LOG("Config Error", "Seriously?! Variable not found, use the \"hasString\" function first");
            return "";
        }
    }


    void ConfigurationManager::setBool(std::string name, bool value)
    {
        intBoolMap[name] = value;
    }

    void ConfigurationManager::setInt(std::string name, int value)
    {
        intBoolMap[name] = value;
    }

    void ConfigurationManager::setDouble(std::string name, double value)
    {
        doubleMap[name] = value;
    }

    void ConfigurationManager::setString(std::string name, std::string value)
    {
        stringMap[name] = value;
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

        cv::FileNode nodes;
        try {
            nodes = configFile.root();
        }
        catch (cv::Exception& e) {
            LOG("Config error", "Error reading configuration file " << filePath);
            return false;
        }

        int nodeType = cv::FileNode::NONE;
        int tempInt;
        double tempDouble;
        cv::string tempString;
        for(cv::FileNodeIterator node = nodes.begin(); node != nodes.end(); node++)
        {

            nodeType = (*node).type();
            switch (nodeType) {
                case cv::FileNode::INT :
                    tempInt = *node;
                    intBoolMap[(*node).name()] = tempInt;
                    nItemsRead++;
                break;
                case cv::FileNode::FLOAT :
                    tempDouble = *node;
                    doubleMap[(*node).name()] = tempDouble;
                    nItemsRead++;
                break;
                case cv::FileNode::STRING :
                    *node >> tempString;
                    stringMap[(*node).name()] = tempString;
                    nItemsRead++;
                    break;
                default:
                    LOG("Config Error", "Variable with name: " << (*node).name() << " is of unknown type");
                    nErrors++;
            }
        }

        LOG("Config", "Configuration file " << filePath << " read successfully with " <<
                      nErrors << " errors out of a total " << nItemsRead << " attempts.");
        nErrors = 0;
        nItemsRead = 0;

        return true;

    }

    bool ConfigurationManager::configure(std::string name, bool &variable) {
        if(!hasBool(name))
            return false;
        variable = getBool(name);
        return true;
    }

    bool ConfigurationManager::configure(std::string name, int &variable) {
        if(!hasInt(name))
            return false;
        variable = getInt(name);
        return true;
    }
    bool ConfigurationManager::configure(std::string name, double &variable) {
        if(!hasDouble(name))
            return false;
        variable = getDouble(name);
        return true;
    }
    bool ConfigurationManager::configure(std::string name, std::string &variable) {
        if(!hasString(name))
            return false;
        variable = getString(name);
        return true;
    }
}
