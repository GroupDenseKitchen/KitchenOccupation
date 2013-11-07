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
    bool manyGtExists = settings.hasStringSeq("groundTruthPaths");
    std::vector<std::string> gtPaths;

    if(manyGtExists) {
        gtPaths = settings.getStringSeq("groundTruthPaths");
    } else {
        return false;
    }

    if (gtPaths.empty()) {
        return false;
    }

    bool threshValExists = settings.hasInt("evalPrecisionThreshold");
    if (!threshValExists) {
        LOG("Evaluation Warning", "No MOTA threshold found, using default");
        trackEvalThreshold = 50;
    } else {
        trackEvalThreshold = settings.getInt("evalPrecisionThreshold");
    }

    bool isTrackEvalInitialized;
    for (unsigned int i = 0; i < gtPaths.size(); i++) {
        TrackerEvaluator* TE = new TrackerEvaluator();
        isTrackEvalInitialized = TE->initialize(gtPaths[i], trackEvalThreshold);

        if (!isTrackEvalInitialized) {
            return false;
        } else {
            trackingEvaluators.push_back(TE);
        }
    }

    return true;
}

void Evaluation::printToLog()
{
    for (unsigned int i = 0; i < trackingEvaluators.size(); i++) {
        trackingEvaluators[i]->printToLog(i);
    }
}

void Evaluation::process(FrameList& frames)
{
    //this->frameList = &frames;
    for (unsigned int i = 0; i < frames.getCurrent().getCameras().size(); i++) {
        trackingEvaluators[i]->process(frames.getCurrent().getCameras()[i]);
    }
}

} // namespace evaluation
