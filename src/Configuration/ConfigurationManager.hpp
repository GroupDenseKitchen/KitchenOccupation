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
 * \details This class reads the settings file and stores all settings
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
         * \return Returns true if successful.
         */
        bool readConfig(std::string filePath);

        /*!
         * \brief       Checks availability of an int-type property
         * \param name  Name of requested property
         * \return      Returns true if specified property exists
         */
        bool hasBool(std::string name);

        /*!
         * \brief       Checks availability of an int-type property
         * \param name  Name of requested property
         * \return      Returns true if specified property exists
         */
        bool hasInt(std::string name);

        /*!
         * \brief       Checks availability of a double-type property
         * \param name  Name of requested property
         * \return      Returns true if specified property exists
         */
        bool hasDouble(std::string name);

        /*!
         * \brief       Checks availability of a string-type property
         * \param item  Name of requested property
         * \return      Returns true if specified property exists
         */
        bool hasString(std::string name);

        /*!
         * \brief       Checks availability of a vector<string>-type property
         * \param item  Name of requested property
         * \return      Returns true if specified property exists
         */
        bool hasStringSeq(std::string name);

        /*!
         * \brief       Gets value of bool with key "name".
         * \param name  Name of the bool variable in question.
         * \return      Value of requested boolean.
         */
        bool getBool(std::string name);

        /*!
         * \brief       Gets value of int with key "name".
         * \param name  Name of the int variable in question.
         * \return      Value of requested int.
         */
        int getInt(std::string name);

        /*!
         * \brief       Gets value of double with key "name".
         * \param name  Name of the double variable in question.
         * \return      Value of requested double.
         */
        double getDouble(std::string name);

        /*!
         * \brief       Gets value of string with key "name".
         * \param name  Name of the string variable in question.
         * \return      Value of requested string.
         */
        std::string getString(std::string name);

        /*!
         * \brief       Gets the string sequence with key "name".
         * \param name  Name of the string sequence in question.
         * \return      Requesed string vector.
         */
        std::vector<std::string> getStringSeq(std::string name);
		
        /*!
         * \brief       Sets value of bool with key "name".
         * \param name  Name of the bool variable in question.
         * \param value New value of the bool variable in question.
         */
        void setBool(std::string name, bool value);

        /*!
         * \brief       Sets value of int with key "name".
         * \param name  Name of the int variable in question.
         * \param value New value of the int variable in question.
         */
        void setInt(std::string name, int value);

        /*!
         * \brief       Sets value of double with key "name".
         * \param name  Name of the double variable in question.
         * \param value New value of the double variable in question.
         */
        void setDouble(std::string name, double value);

        /*!
         * \brief       Sets value of string with key "name".
         * \param name  Name of the string variable in question.
         * \param value New value of the string variable in question.
         */
        void setString(std::string name, std::string value);

        /*!
         * \brief       Sets value of string sequence with key "name".
         * \param name  Name of the string sequence variable in question.
         * \param value New value of the string sequence in question.
         */
        void setStringSeq(std::string name, std::vector<std::string> value);

        /*!
         * \brief       Sets variable if it exists otherwise return false.
         */
        bool configure(std::string name, bool &variable);
		
        /*!
         * \brief       Sets variable if it exists otherwise return false.
         */
        bool configure(std::string name, int &variable);
		
        /*!
         * \brief       Sets variable if it exists otherwise return false.
         */
        bool configure(std::string name, double &variable);
		
        /*!
         * \brief       Sets variable if it exists otherwise return false.
         */
        bool configure(std::string name, std::string &variable);

        /*!
         * \brief       Writes all stored settings in the configuration manager to file.
         * \param file  Location and name of output file.
         * \return      Returns true if successful.
         */
        bool writeToFile(std::string filePath);

    private:
        // Reading functions. (Templates won't work because OpenCV is not super awesome)

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

        std::map<std::string, int> intBoolMap;
        std::map<std::string, double> doubleMap;
        std::map<std::string, std::string> stringMap;
        std::map<std::string, std::vector<std::string> > stringSeqMap;



};

} // namespace

#endif
