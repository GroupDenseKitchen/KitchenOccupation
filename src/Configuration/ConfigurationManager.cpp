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
        return isBool;
    }
    bool ConfigurationManager::hasInt(std::string name)
    {
        bool exists = intBoolMap.find(name) != intBoolMap.end();
        return exists;
    }
    bool ConfigurationManager::hasDouble(std::string name)
    {
        bool exists = doubleMap.find(name) != doubleMap.end();
        return exists;
    }

    bool ConfigurationManager::hasString(std::string name)
    {
        bool exists = stringMap.find(name) != stringMap.end();
        return exists;
    }

    bool ConfigurationManager::hasStringSeq(std::string name)
    {
        bool exists = stringSeqMap.find(name) != stringSeqMap.end();
        return exists;
    }

    bool ConfigurationManager::getBool(std::string name)
    {
        if (hasBool(name)) {
            return (bool)intBoolMap[name];
        } else {
            LOG("Settings Error", "Variable not found, use the \"hasBool\" function first");
            return false;
        }
    }

    int ConfigurationManager::getInt(std::string name)
    {
        if (hasInt(name)) {
            return intBoolMap[name];
        } else {
            LOG("Settings Error", "Variable not found, use the \"hasInt\" function first");
            return 0;
        }
    }

    double ConfigurationManager::getDouble(std::string name)
    {
        if (hasDouble(name)) {
            return doubleMap[name];
        } else {
            LOG("Settings Error", "Variable not found, use the \"hasDouble\" function first");
            return 0;
        }
    }

    std::string ConfigurationManager::getString(std::string name)
    {
        if (hasString(name)) {
            return stringMap[name];
        } else {
            LOG("Settings Error", "Variable not found, use the \"hasString\" function first");
            return "";
        }
    }

    std::vector<std::string> ConfigurationManager::getStringSeq(std::string name)
    {
        if (hasStringSeq(name)) {
            return stringSeqMap[name];
        } else {
            LOG("Settings Error", "Variable not found, use the \"hasStringSeq\" function first");
            std::vector<std::string> emptyVec;
            return emptyVec;
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

    void ConfigurationManager::setStringSeq(std::string name, std::vector<std::string> value)
    {
        stringSeqMap[name] = value;
    }


    bool ConfigurationManager::readConfig(std::string filePath)
    {
        nErrors = 0;
        nItemsRead = 0;
        try {
            configFile.open(filePath,cv::FileStorage::READ);
        }
        catch (cv::Exception& e) {
            LOG("Settings Error", "Error reading configuration file " << filePath);
            return false;
        }

        cv::FileNode nodes;
        try {
            nodes = configFile.root();
        }
        catch (cv::Exception& e) {
            LOG("Settings Error", "Error reading configuration file " << filePath);
            return false;
        }

        int nodeType = cv::FileNode::NONE;
        int tempInt;
        double tempDouble;
        cv::string tempString;
        std::vector<std::string> tempStringVec;
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
                case cv::FileNode::SEQ :
                    *node >> tempStringVec;
                    stringSeqMap[(*node).name()] = tempStringVec;
                    tempStringVec.clear();
                    nItemsRead++;
                    break;
                default:
                    LOG("Settings Error", "Variable with name: " << (*node).name() << " is of unknown type");
                    nErrors++;
            }
        }

        LOG("Settings", "Configuration file " << filePath << " read successfully with " <<
                      nErrors << " errors out of a total " << nItemsRead << " attempts.");
        nErrors = 0;
        nItemsRead = 0;
        cfgFilePath = filePath;

        configFile.release();
        return true;

    }

    bool ConfigurationManager::configure(std::string name, bool &variable) {
        if(!hasBool(name)) {
            setBool(name, variable);
            return false;
        }
        variable = getBool(name);
        return true;
    }

    bool ConfigurationManager::configure(std::string name, int &variable) {
        if(!hasInt(name)) {
            setInt(name, variable);
            return false;
        }
        variable = getInt(name);
        return true;
    }

    bool ConfigurationManager::configure(std::string name, double &variable) {
        if(!hasDouble(name)) {
            setDouble(name, variable);
            return false;
        }
        variable = getDouble(name);
        return true;
    }
    bool ConfigurationManager::configure(std::string name, std::string &variable) {
        if(!hasString(name)) {
            setString(name, variable);
            return false;
        }
        variable = getString(name);
        return true;
    }

    bool ConfigurationManager::writeToFile()
    {
        cv::FileStorage file;
        std::string outputPath = cfgFilePath;
        replaceString(outputPath, ".yml", "_out.yml");

        try {
            file.open(outputPath, cv::FileStorage::WRITE);
        }
        catch (cv::Exception& e) {
            LOG("Configuration Error", "Error opening file for writing at: " << outputPath << ".");
            return false;
        }

        // All ints and booleans
        for (std::map<std::string, int>::iterator intBool = intBoolMap.begin(); intBool != intBoolMap.end(); intBool++) {
            try {
                file << intBool->first << intBool->second;
            }
            catch (cv::Exception& e) {
                LOG("Configuration Error", "Error writing int named: " << intBool->first <<
                                           " with value: " << intBool->second << " to file " << outputPath << ".");
            }
        }

        // All doubles
        for (std::map<std::string, double>::iterator dbl = doubleMap.begin(); dbl != doubleMap.end(); dbl++) {
            try {
                file << dbl->first << dbl->second;
            }
            catch (cv::Exception& e) {
                LOG("Configuration Error", "Error writing double named: " << dbl->first <<
                                           " with value: " << dbl->second << " to file " << outputPath << ".");
            }
        }

        // All strings
        for (std::map<std::string, std::string>::iterator str = stringMap.begin(); str != stringMap.end(); str++) {
            try {
                file << str->first << str->second;
            }
            catch (cv::Exception& e) {
                LOG("Configuration Error", "Error writing string named: " << str->first <<
                                           " with value: " << str->second << " to file " << outputPath << ".");
            }
        }

        // All string sequences
        for (std::map<std::string, std::vector<std::string>>::iterator stringSeq = stringSeqMap.begin(); stringSeq != stringSeqMap.end(); stringSeq++) {
            try {
                file << stringSeq->first << stringSeq->second;
            }
            catch (cv::Exception& e) {
                LOG("Configuration Error", "Error writing string sequence named: " << stringSeq->first <<
                                           " to file " << outputPath << ".");
            }
        }

        file.release();
        return true;
    }
}
