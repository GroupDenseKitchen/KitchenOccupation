#include "EntryExitEvaluator.hpp"

namespace evaluation {


    EntryExitEvaluator::EntryExitEvaluator()
    {
    }

    EntryExitEvaluator::~EntryExitEvaluator()
    {
    }

    bool EntryExitEvaluator::initialize(configuration::ConfigurationManager &settings)
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
        int minFrameCount = INT_MAX;
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
            std::vector<inOutEvent> currentCamera;
            inOutEvent inOut;
            std::cout << in.size();
            for(std::vector<int>::size_type n = 0; n != in.size(); n++) {
                inOut.in = in[n];
                inOut.out = out[n];
                currentCamera.push_back(inOut);

            }
            minFrameCount = cv::min(minFrameCount, (int)currentCamera.size());
            groundTruth.push_back(currentCamera);
        }

        // Set frame count
        frameCount = 0;

        for (std::vector<std::string>::size_type i = 0; i != entryExitGtPaths.size(); i++) {
            prevEntered.push_back(0);
            prevExited.push_back(0);
            sumEntryGT.push_back(0);
            sumExitGT.push_back(0);
            diffEntries.push_back(0);
            diffExits.push_back(0);
            diffTotalOfPeople.push_back(0);

        }
        numberOfFrames = minFrameCount;

        return true;
    }

    void EntryExitEvaluator::process(FrameList &frames)
    {        
        bool asdf;
        if (frameCount > 309) {
            asdf = true;
        }
        frameCount++;
        if (frameCount > numberOfFrames)
            return;

        std::vector<inOutEvent> inOut;
        for (std::vector<CameraObject>::size_type i = 0;
             i < frames.getCurrent().getCameras().size();
             i++) {
            inOutEvent temp;
            temp.in = 0;
            temp.out = 0;
            inOut.push_back(temp);
        }

        // Save the values for entry and exit
        for (std::vector<CameraObject>::size_type i = 0;
             i < frames.getCurrent().getCameras().size();
             i++) {

            // getEntered/getExited is the sum of all entrys and exits.
            inOut[i].in = frames.getCurrent().getCameras()[i].getEntered() - prevEntered[i];
            inOut[i].out = frames.getCurrent().getCameras()[i].getExited() - prevExited[i];
            //entryExitData[i].push_back(inOut[i]);

            // Save current values for next iteration
            prevEntered[i] = frames.getCurrent().getCameras()[i].getEntered();
            prevExited[i] = frames.getCurrent().getCameras()[i].getExited();

            // Sum ground truth enties and exits
            sumEntryGT[i] = sumEntryGT[i] + groundTruth[i][frameCount].in;
            sumExitGT[i] = sumExitGT[i] + groundTruth[i][frameCount].out;

            // Evaluation
            // Diffrence in entries and exits and total number of people in the room
            diffEntries[i] = frames.getCurrent().getCameras()[i].getEntered() - sumEntryGT[i];
            diffExits[i] = frames.getCurrent().getCameras()[i].getExited() - sumExitGT[i];
            diffTotalOfPeople[i] = (frames.getCurrent().getCameras()[i].getEntered() -
                                    frames.getCurrent().getCameras()[i].getExited()) -
                                    (sumEntryGT[i] - sumExitGT[i]);


            // Debug
            // Print entry and exit information on the image
            cv::Mat raw = frames.getCurrent().getCameras()[i].getImage("rawImage");
            std::string textEntryGT = "";
            std::string textExitGT = "";
            std::string textEntryDiff = "";
            std::string textExitDiff = "";
            std::string textTotalDiff = "";
            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 1;
            int thickness = 1;
            cv::Point2d pos1(300,15);
            cv::Point2d pos2(300,35);
            cv::Point2d pos3(300,55);
            cv::Point2d pos4(300,75);
            cv::Point2d pos5(300,95);
            textEntryGT = "Entered GT: " + std::to_string(sumEntryGT[i]);
            textExitGT = "Exited GT: " + std::to_string(sumExitGT[i]);
            textEntryDiff = "Entered difference: " + std::to_string(diffEntries[i]);
            textExitDiff = "Exited difference: " + std::to_string(diffExits[i]);
            textTotalDiff  = "Total difference: " + std::to_string(diffTotalOfPeople[i]);
            putText(raw, textEntryGT, pos1, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textExitGT, pos2, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textEntryDiff, pos3, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textExitDiff, pos4, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textTotalDiff, pos5, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
        }
    }

    void EntryExitEvaluator::printToLog(unsigned int cameraIndex)
    {
        LOG("Evaluation Entry/Exit", "Difference in entries of camera " + std::to_string(cameraIndex) + " is: " + std::to_string(diffEntries[cameraIndex]));
        LOG("Evaluation Entry/Exit", "Difference in exits of camera " + std::to_string(cameraIndex) + " is: " + std::to_string(diffExits[cameraIndex]));
        LOG("Evaluation Entry/Exit", "Difference in total number of people in room of camera "
            + std::to_string(cameraIndex) + " is: " + std::to_string(diffTotalOfPeople[cameraIndex]));
    }

} //namespace evaluation
