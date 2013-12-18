#include "EntryExitEvaluator.hpp"

namespace evaluation {


EntryExitEvaluator::EntryExitEvaluator()
{
}

EntryExitEvaluator::~EntryExitEvaluator()
{
}

#ifndef __APPLE__
int round(float x) {
    return std::floor(x+0.5);
}
#endif

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
        LOG("EntryExitEvauator", "Ground truth specified in \"entryExitGroundTruthPaths:\"");
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
    accuracyIn = 0;
    accuracyOut = 0;
    accuracyTot = 0;
    save = 10; //save every 10th frame
    return true;
}

void EntryExitEvaluator::process(FrameList &frames)
{
    frameCount++;
    if (frameCount > numberOfFrames) {
        return;
    }

    std::vector<inOutEvent> inOut;
    for (std::vector<CameraObject>::size_type i = 0; i < frames.getCurrent().getCameras().size(); i++) {
        inOutEvent temp;
        temp.in = 0;
        temp.out = 0;
        inOut.push_back(temp);
    }

    // Save the values for entry and exit
    for (std::vector<CameraObject>::size_type i = 0; i < frames.getCurrent().getCameras().size(); i++) {
        // getEntered/getExited is the sum of all entrys and exits.
        inOut[i].in = frames.getCurrent().getCameras()[i].getEntered() - prevEntered[i];
        inOut[i].out = frames.getCurrent().getCameras()[i].getExited() - prevExited[i];

        // Save current values for next iteration
        prevEntered[i] = frames.getCurrent().getCameras()[i].getEntered();
        prevExited[i] = frames.getCurrent().getCameras()[i].getExited();



        // Sum ground truth enties and exits
        sumEntryGT[i] = sumEntryGT[i] + groundTruth[i][frameCount].in;
        sumExitGT[i] = sumExitGT[i] + groundTruth[i][frameCount].out;

        //---------- Save data -----------
        if(save == 10) {
        saveAccuracyToFile(prevEntered[i], "/Users/erikfall/Desktop/entMeas.csv");
        saveAccuracyToFile(prevExited[i], "/Users/erikfall/Desktop/exiMeas.csv");

        saveAccuracyToFile(sumEntryGT[i], "/Users/erikfall/Desktop/entryGT.csv");
        saveAccuracyToFile(sumExitGT[i], "/Users/erikfall/Desktop/exitGT.csv");
        //--------------------------------
        }


        //Accuracy computation
        if(sumEntryGT[i] != 0 && sumExitGT[i] != 0){
            accuracyTot = 1 - float(std::abs(frames.getCurrent().getCameras()[i].getEntered()-
                                             frames.getCurrent().getCameras()[i].getExited()-
                                             (sumEntryGT[i]-sumExitGT[i])))/(sumEntryGT[i]+sumExitGT[i]);
            accuracyIn  = 1 - float(std::abs(frames.getCurrent().getCameras()[i].getEntered()-sumEntryGT[i])) / sumEntryGT[i];
            accuracyOut = 1 - float(std::abs(frames.getCurrent().getCameras()[i].getExited()-sumExitGT[i])) / sumExitGT[i];

            diffEntries[i]       = sumEntryGT[i] - frames.getCurrent().getCameras()[i].getEntered();
            diffExits[i]         = sumExitGT[i] - frames.getCurrent().getCameras()[i].getExited();
            diffTotalOfPeople[i] = (sumEntryGT[i] - sumExitGT[i])-
                    (frames.getCurrent().getCameras()[i].getEntered() - frames.getCurrent().getCameras()[i].getExited());


            // Debug, print entry and exit information on the image.
            if(frames.getCurrent().getCameras()[i].hasImage("debugImage"))
            {
                cv::Mat debugIm = frames.getCurrent().getCameras()[i].getImage("debugImage");
                std::string accIn = "";
                std::string accOut = "";
                std::string accTotDiff = "";
                std::string GTIn = "";
                std::string GTOut = "";
                std::string GTTotal = "";

                int fontFace     = cv::FONT_HERSHEY_PLAIN;
                double fontScale = 1;
                int thickness    = 1;

                cv::Point2d pos1(430,15);
                cv::Point2d pos2(430,35);
                cv::Point2d pos3(430,55);
                cv::Point2d pos4(140,20);
                cv::Point2d pos5(140,40);
                cv::Point2d pos6(140,60);

                accIn      = "Accuracy In:      " + std::to_string(int(round(accuracyIn*100))) + " %";
                accOut     = "Accuracy Out:     " + std::to_string(int(round(accuracyOut*100))) + " %";
                accTotDiff = "Accuracy TotDiff: " + std::to_string(int(round(accuracyTot*100))) + " %";
                GTIn       = "(" + std::to_string(sumEntryGT[i]) + ")";
                GTOut      = "(" + std::to_string(sumExitGT[i]) + ")";
                GTTotal    = "(" + std::to_string(sumEntryGT[i] - sumExitGT[i]) + ")";

                putText(debugIm, accIn,      pos1, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
                putText(debugIm, accOut,     pos2, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
                putText(debugIm, accTotDiff, pos3, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
                putText(debugIm, GTIn,       pos4, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
                putText(debugIm, GTOut,      pos5, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
                putText(debugIm, GTTotal,    pos6, fontFace, fontScale, cv::Scalar(0,255,0), thickness, 8);
            }
        }

        if(save == 10) {
        saveAccuracyToFile(accuracyIn, "/Users/erikfall/Desktop/accuracyIn.csv");
        saveAccuracyToFile(accuracyOut, "/Users/erikfall/Desktop/accuracyOut.csv");
        saveAccuracyToFile(accuracyTot, "/Users/erikfall/Desktop/accuracyTot.csv");
        }
        if(save == 10) {
            save = 0;
        }
        ++ save;

    }
}

void EntryExitEvaluator::saveAccuracyToFile(float value, std::string filepath)
{
    std::ofstream myfile;
     myfile.open(filepath,std::ios_base::app);
     myfile << value << ",";
     myfile.close();
}

void EntryExitEvaluator::printToLog(unsigned int cameraIndex)
{
    LOG("Evaluation Entry/Exit", "Difference in entries of camera " + std::to_string(cameraIndex) + " is: " + std::to_string(diffEntries[cameraIndex]));
    LOG("Evaluation Entry/Exit", "Difference in exits of camera " + std::to_string(cameraIndex) + " is: " + std::to_string(diffExits[cameraIndex]));
    LOG("Evaluation Entry/Exit", "Difference in total number of people in room of camera "
        + std::to_string(cameraIndex) + " is: " + std::to_string(diffTotalOfPeople[cameraIndex]));
}

} //namespace evaluation
