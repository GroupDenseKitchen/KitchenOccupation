#include "ImageProcessor.hpp"

namespace image_processing
{
    ImageProcessor::ImageProcessor(){

    }

    ImageProcessor::~ImageProcessor(){

    }

    bool ImageProcessor::initialize(configuration::ConfigurationManager & config)
    {
        /*
        std::vector<std::string> algorithms = config.getStringSequence("ImageProcessor");
        for(std::vector<std::string>::iterator algorithm = algorithms.begin(); algorithm != algorithms.end(); algorithm++)
            if(AlgorithmFactory::has(*algorithm))
                addAlgorithm(*algorithm, AlgorithmFactory::get(*algorithm));
            else    //TODO: Add support to recursivly add sub-algorithms...
                LOG("ImageProcessor initialize error", "Algorithm \"" << *algorithm << "\" not found!");
*/
        addAlgorithm("Background modeling",         new BackgroundModel());
        addAlgorithm("Foreground region extractor", new ForegroundRegionExtractor());
        addAlgorithm("Tracking",                    new Tracking());
        return Algorithm::initialize(config);   // Initialize all algorithms
    }

    void ImageProcessor::process(FrameList &frames)
    {
        PROFILER_START("Image Processing");
        Algorithm::process(frames);
        PROFILER_END();
    }
}
