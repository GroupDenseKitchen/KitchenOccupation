#include "ImageProcessor.hpp"

namespace image_processing
{
    ImageProcessor::ImageProcessor(){

    }

    ImageProcessor::~ImageProcessor(){

    }

    bool ImageProcessor::initialize(configuration::ConfigurationManager & configuration)
    {
        addAlgorithm("Background modeling",         BackgroundModel());
        addAlgorithm("Foreground region extractor", ForegroundRegionExtractor());
        addAlgorithm("Tracking",                    Tracking());
        Algorithm::initialize(configuration);   // Initialize all algorithms
    }
}
