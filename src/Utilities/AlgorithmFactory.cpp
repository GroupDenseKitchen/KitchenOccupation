#include "AlgorithmFactory.hpp"

void AlgorithmFactory::clear() {
    algorithms.clear(); //TODO: delete all algorithms first
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
