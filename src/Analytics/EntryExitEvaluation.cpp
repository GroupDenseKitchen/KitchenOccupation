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
        for(unsigned int i = 0; i < entryExitGtPaths.size(); i++) {
            //Läs in aktuell in/ut gt för aktuell kamera
            std::vector<int> in; //=...hur man läser från yml-fil
            std::vector<int> out;

            // Loopa igenom vektorna in och ut
            std::vector<inOutEvent> actualCamera;
            inOutEvent inOut;
            for(std::vector<int>::size_type n = 0; n != in.size(); n++) {
                inOut.in = in[n];
                inOut.out = out[n];
                actualCamera.push_back(inOut);
            }
            groundTruth.push_back(actualCamera);
        }

        //LOG("EntryExitEvaluation Error", "Failed to read ground truth file at: " << groundTruthPath)

        return true;
    }

    void EntryExitEvaluation::process(FrameList &frames)
    {
        std::vector<int> entered;
        std::vector<int> exited;

        for (std::vector<CameraObject>::iterator i = frames.getCurrent().getCameras().begin();
             i != frames.getCurrent().getCameras().end();
             ++i) {
            entered.push_back(i->getEntered());
            exited.push_back(i->getExited());
        }
    }

} //namespace evaluation
