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
        bool gtExists = settings.hasStringSeq("entryExitGroundTruthPaths");
        std::vector<std::string> entryExitGtPaths;

        frameCount = 0;

        if(gtExists) {
            entryExitGtPaths = settings.getStringSeq("entryExitGroundTruthPaths");
        } else {
            return false;
        }

        if(entryExitGtPaths.empty()) {
            return false;
        }

        //Stega igenom listan med filer/kamror
        //Läs in ground truth för varje frame för varje kamera
        for(std::vector<std::string>::size_type i = 0; i != entryExitGtPaths.size(); i++) {
            //Läs in aktuell in/ut gt för aktuell kamera
            std::string source = entryExitGtPaths[i];
            cv::FileStorage file(source, cv::FileStorage::READ);
            std::vector<int> in;
            std::vector<int> out;
            file["Entries"] >> in;
            file["Exits"] >> out;

            // Loopa igenom vektorna in och ut
            std::vector<inOutEvent> actualCamera;
            inOutEvent inOut;
            std::cout << in.size();
            for(std::vector<int>::size_type n = 0; n != in.size(); n++) {
                inOut.in = in[n];
                inOut.out = out[n];
                //std::cout << in[n] << "  " << out[n] << std::endl;
                actualCamera.push_back(inOut);
            }
            groundTruth.push_back(actualCamera);
        }

        //LOG("EntryExitEvaluation Error", "Failed to read ground truth file at: " << groundTruthPath)

        //Testar om det funkar...
        //for(std::vector<vector<>>::)

        return true;
    }

    void EntryExitEvaluation::process(FrameList &frames)
    {        
        //Hur blir det om man stegar bakåt med denna vektor??
        std::vector<inOutEvent> actualCamera;
        inOutEvent inOut;

        this->frameCount++;

        //getEntered/getExited är summan av totala hittills
        //för att få för aktuell frame, ta diff mellan getCurrent och getPreviuous

        int prevEntered = 0;
        int prevExited = 0;

        // VERKAR JU INTE SÄTTAS NÅGONSTANS I PROGRAMMET!
        //Sparar undan värdena för entry & exit
        for (std::vector<CameraObject>::size_type i = 0;
             i < frames.getCurrent().getCameras().size();
             i++) {

            //Om första framen
            if(!frames.hasPrevious()){ //onödigt? prevEntered/prevExited = 0
                inOut.in = frames.getCurrent().getCameras()[i].getEntered();
                inOut.out = frames.getCurrent().getCameras()[i].getExited();
                actualCamera.push_back(inOut);
            }
            //Om inte första framen
            else {
                inOut.in = frames.getCurrent().getCameras()[i].getEntered() - prevEntered;
                inOut.out = frames.getCurrent().getCameras()[i].getExited() - prevExited;
                actualCamera.push_back(inOut);
            }

            // Skriver ut på bilden så man ser om det funkar...
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
            textEntry = "Entered: " + std::to_string(frames.getCurrent().getCameras()[i].getEntered() - prevEntered); //actualCamera[this->frameCount].in funkar konstigt...
            textExit = "Exited: " + std::to_string(frames.getCurrent().getCameras()[i].getExited() - prevExited); //actualCamera[this->frameCount].out funkar konstigt...
            textEntryGT = "Entered GT: " + std::to_string(groundTruth[i].size()); //groundTruth[i][frameCount].in
            textExitGT = "Exited GT: " + std::to_string(frameCount); //groundTruth[i][frameCount].out
            putText(raw, textEntry, pos1, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textExit, pos2, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textEntryGT, pos3, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            putText(raw, textExitGT, pos4, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);

            //Sparar undan aktuellt värde till nästa iteration
            prevEntered = frames.getCurrent().getCameras()[i].getEntered();
            prevExited = frames.getCurrent().getCameras()[i].getExited();
        }
        // Evaluerar...
    }

} //namespace evaluation
