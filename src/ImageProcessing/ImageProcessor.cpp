#include "ImageProcessor.hpp"

namespace image_processing
{
    ImageProcessor::ImageProcessor() {}

    ImageProcessor::~ImageProcessor() {}

    bool ImageProcessor::initialize(configuration::ConfigurationManager& settings) {
        return Algorithm::initialize(settings);   // Initialize all algorithms
    }

    void ImageProcessor::reset()
    {
        algorithms.clear();
        algorithmTag.clear();
    }

    void ImageProcessor::process(FrameList& frames)
    {
        Algorithm::process(frames);
    }
}
