int main()
{
	
    // Settings
    bool isDebug = 0;                   // Running debug GUI?
    bool isTesting = 1;                 // Running from video file?
    bool hasGroundTruth = 0;            // Has labeled ground truth data?

    int nCameras = 1;                   // Number of cameras/video files.

    std::string videoFilePath = "data/someFile00.avi";          // Location of video file(s).
    std::string groundTruthPath = "data/someFile00GT.xml";      // Location of ground truth data for specified video

    cv::FileStorage cfgFile("settings.yml", cv::FileStorage::WRITE);

    cfgFile << "isDebug" << isDebug;
    cfgFile << "isTesting" << isTesting;
    cfgFile << "hasGroundTruth" << hasGroundTruth;
    cfgFile << "nCameras" << nCameras;
    cfgFile << "videoFilePath" << videoFilePath;
    cfgFile << "groundTruthPath" << groundTruthPath;

    cfgFile.release();

	
    DenseKitchen program;
    program.init();
    program.readConfig("settings.yml");
    debugging::logObject.dumpToConsole();
    return 0;
}	


	
