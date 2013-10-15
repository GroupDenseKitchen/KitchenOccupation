#include "AlgorithmFactory.hpp"


// Included algorithms:
#include "../ImageProcessing/ImageProcessor.hpp"
#include "../ImageProcessing/BackgroundModel.hpp"
#include "../ImageProcessing/ForegroundRegionExtractor.hpp"
#include "../ImageProcessing/Tracking.hpp"

#include "../Analytics/Analytics.hpp"

using namespace image_processing;
using namespace statistics;


void AlgorithmFactory::initialize() {
    // Here goes algorithms that should be registered..
    add("ImageProcessor",               new ImageProcessor());
    add("BackgroundModel",              new BackgroundModel());
    add("ForegroundRegionExtractor",    new ForegroundRegionExtractor());
    add("Tracking",                     new Tracking());

    add("Analytics",                    new Analytics());
}

bool AlgorithmFactory::has(std::string algorithmClassName) {
    return algorithms.find(algorithmClassName) != algorithms.end();
}

Algorithm * AlgorithmFactory::get(std::string algorithmClassName) {
    return algorithms[algorithmClassName];
}

void AlgorithmFactory::add(std::string algorithmClassName, Algorithm * algorithm) {
    algorithms[algorithmClassName] = algorithm;
}
