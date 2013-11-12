#include "Evaluation.hpp"

using namespace rapidxml;

namespace evaluation
{

Evaluation::Evaluation() {}

Evaluation::~Evaluation()
{
    trackingEvaluators.clear();
}

bool Evaluation::initialize(configuration::ConfigurationManager &settings)
{
    if (!settings.hasBool("runFromFile") || !settings.getBool("runFromFile")) {
        LOG("Evaluation", "Running from live stream, evaluation unavailable.");
        return false;
    }

    bool gtExists = settings.hasStringSeq("trackerGroundTruthPaths");
    std::vector<std::string> trackerGtPaths;

    if(gtExists) {
        trackerGtPaths = settings.getStringSeq("trackerGroundTruthPaths");
    } else {
        return false;
    }

    if (trackerGtPaths.empty()) {
        return false;
    }

    bool threshValExists = settings.hasInt("evalPrecisionThreshold");
    if (!threshValExists) {
        LOG("Evaluation Warning", "No MOTA threshold found, using default");
        trackEvalThreshold = 50;
    } else {
        trackEvalThreshold = settings.getInt("evalPrecisionThreshold");
    }

    for (unsigned int i = 0; i < trackerGtPaths.size(); i++) {
        TrackerEvaluator* TE = new TrackerEvaluator();
        bool isTrackEvalInitialized = TE->initialize(trackerGtPaths[i], trackEvalThreshold);

        if (!isTrackEvalInitialized) {
            trackingEvaluators.push_back(0);
        } else {
            trackingEvaluators.push_back(TE);
        }
    }

    return true;
}

void Evaluation::printToLog()
{    
    for (unsigned int i = 0; i < trackingEvaluators.size(); i++) {
        if(trackingEvaluators[i]) {
            trackingEvaluators[i]->printToLog(i);
        }
    }
}

void Evaluation::process(FrameList& frames)
{
    for (unsigned int i = 0; i < frames.getCurrent().getCameras().size(); i++) {
        if(trackingEvaluators[i]) {
            trackingEvaluators[i]->process(frames.getCurrent().getCameras()[i]);
        }
    }
}

} // namespace evaluation
