#include "EntryExitEvaluation.hpp"

namespace evaluation {

    EntryExitEvaluation::EntryExitEvaluation()
    {
    }

    EntryExitEvaluation::~EntryExitEvaluation()
    {
    }

    bool EntryExitEvaluation::initialize(configuration::ConfigurationManager &settings)
    {
        // Reads ground truth from files.
        // Check if ground truth exists.
        bool gtExists = settings.hasStringSeq("entryExitGroundTruthPaths");
        std::vector<std::string> entryExitGtPaths;

        // If ground truth exists, read the path strings. Else return false.
        if(gtExists) {
            entryExitGtPaths = settings.getStringSeq("entryExitGroundTruthPaths");
        } else {
            return false;
        }

        // If ground truth path is emty, return false.
        if(entryExitGtPaths.empty()) {
            return false;
        }

        // For each camera, read the ground truth (entry/exit)
        for(std::vector<std::string>::size_type i = 0; i != entryExitGtPaths.size(); i++) {
            // Read from file
            std::string source = entryExitGtPaths[i];
            cv::FileStorage file(source, cv::FileStorage::READ);
            std::vector<int> in;
            std::vector<int> out;
            file["Entries"] >> in;
            file["Exits"] >> out;

            // Save the ground truth values in groundTrouth.
            std::vector<inOutEvent> actualCamera;
            inOutEvent inOut;
            std::cout << in.size();
            for(std::vector<int>::size_type n = 0; n != in.size(); n++) {
                inOut.in = in[n];
                inOut.out = out[n];
                actualCamera.push_back(inOut);
            }
            groundTruth.push_back(actualCamera);
        }

        // Set frame count
        frameCount = 0;

        return true;
    }

    void EntryExitEvaluation::process(FrameList &frames)
    {        
        // Hur blir det om man stegar bakåt med denna vektor??
        // Fast stega bakåt verkar inte funka...
        std::vector<inOutEvent> actualCamera;
        inOutEvent inOut;

        frameCount++;

        int prevEntered = 0;
        int prevExited = 0;

        // VERKAR JU INTE SÄTTAS NÅGONSTANS I PROGRAMMET!
        // Save the values for entry and exit
        for (std::vector<CameraObject>::size_type i = 0;
             i < frames.getCurrent().getCameras().size();
             i++) {

            // getEntered/getExited is the sum of all entrys and exits.
            inOut.in = frames.getCurrent().getCameras()[i].getEntered() - prevEntered;
            inOut.out = frames.getCurrent().getCameras()[i].getExited() - prevExited;
            actualCamera.push_back(inOut);

            // Save current values for next iteration
            prevEntered = frames.getCurrent().getCameras()[i].getEntered();
            prevExited = frames.getCurrent().getCameras()[i].getExited();

            // Debug
            // Print entry and exit information on the image
            // Has to step trough the frames to see the entries and exits, they are only visible in one frame
            cv::Mat raw = frames.getCurrent().getCameras()[i].getImage("rawImage");
            std::string textEntry = "";
            std::string textExit = "";
            std::string textEntryGT = "";
            std::string textExitGT = "";
            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 1;
            int thickness = 1;
            cv::Point2d pos1(300,15);
            cv::Point2d pos2(300,35);
            cv::Point2d pos3(300,55);
            cv::Point2d pos4(300,75);
            textEntry = "Entered: " + std::to_string(inOut.in);
            textExit = "Exited: " + std::to_string(inOut.out);
            textEntryGT = "Entered GT: " + std::to_string(groundTruth[i][frameCount].in);
            textExitGT = "Exited GT: " + std::to_string(groundTruth[i][frameCount].out);
            putText(raw, textEntry, pos1, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textExit, pos2, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textEntryGT, pos3, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textExitGT, pos4, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
        }

        // Evaluation?
    }

} //namespace evaluation
