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


        /*!
         * \brief Do we run the debug GUI?
         */
        bool getDebugStatus();
        /*!
         * \brief Do we load video from files?
         */
        bool getTestingStatus();
        /*!
         * \brief Do we have ground truth data for the file.
         */
        bool getGroundTruthStatus();

        /*!
         * \brief Returns the total number of cameras/video files.
         */
        int getNumberOfCameras();

        /*!
         * \brief Get path to video file.
         */
        std::string getVideoFilePath();
        /*!
         * \brief Get path to XML ground truth file.
         */
        std::string getGroundTruthPath();

    private:
        // Reading functions. (Templates won't work because OpenCV is not super awesome)
        void readItem(std::string itemNameInFile, bool item);
        void readItem(std::string itemNameInFile, int item);
        void readItem(std::string itemNameInFile, float item);
        void readItem(std::string itemNameInFile, std::string item);
        void readItem(std::string itemNameInFile, cv::Mat item);

        int nItemsRead;     // Total number of items read
        int nErrors;        // Total number of errors

        cv::FileStorage configFile;     // OpenCV file management class

        // Settings
        bool isDebug;                   // Running debug GUI?
        bool isTesting;                 // Running from video file?
        bool hasGroundTruth;            // Has labeled ground truth data?

        int nCameras;                   // Number of cameras/video files.

        std::string videoFilePath;      // Location of video file(s).
        std::string groundTruthPath;    // Location of ground truth data for specified video

};

} // namespace

#endif
