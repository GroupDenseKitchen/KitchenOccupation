#include "ImageProcessor.hpp"

namespace image_processing
{
    ImageProcessor::ImageProcessor(){

    }

    ImageProcessor::~ImageProcessor(){

    }

    bool ImageProcessor::initialize(configuration::ConfigurationManager & config)
    {
        return Algorithm::initialize(config);   // Initialize all algorithms
    }

    void ImageProcessor::process(FrameList &frames)
    {
        PROFILER_START("Image Processing");
        Algorithm::process(frames);
        PROFILER_END();
    }
}
