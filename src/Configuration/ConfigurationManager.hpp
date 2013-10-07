#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "../Utilities/utilities.hpp"

/*!
 * \brief Namespace enveloping application settings.
 */
namespace configuration
{
/*!
 * \brief Manages application settings.
 * \details This class reads the config file and stores all settings
 * as local variables.
 *
 */
class ConfigurationManager
{
    public:
        /*!
         * \brief Empty constructor
         */
        ConfigurationManager();
        /*!
         * \brief Destructor
         */
        ~ConfigurationManager();
        /*!
         * \brief Reads a configuration file.
         * \param Location of configuration file.
         * \return Resturns true if successful.
         */
        bool readConfig(std::string filePath);



    private:
        void readItem(std::string itemName, bool item);
        void readItem(std::string itemName, int item) {}
        void readItem(std::string itemName, std::string item) {}

        int errors;

        cv::FileStorage configFile;
        bool isDebug, isTesting;
        int nCameras;               // Also specifies the number of video files.
        std::string videoFilePath;


};

} // namespace

#endif
