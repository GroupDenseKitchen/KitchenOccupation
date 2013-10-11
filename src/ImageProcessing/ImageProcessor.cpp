#include "ImageProcessor.hpp"

namespace image_processing
{
    ImageProcessor::ImageProcessor(){

    }

    ImageProcessor::~ImageProcessor(){

    }

    bool ImageProcessor::initialize(configuration::ConfigurationManager & configuration)
    {
        addAlgorithm("Background modeling",         new BackgroundModel());
        addAlgorithm("Foreground region extractor", new ForegroundRegionExtractor());
        addAlgorithm("Tracking",                    new Tracking());
        Algorithm::initialize(configuration);   // Initialize all algorithms
    }

    void ImageProcessor::process(FrameList &frames)
    {
        PROFILER_START("Image Processing");
        Algorithm::process(frames);
        PROFILER_END();
    }
}
