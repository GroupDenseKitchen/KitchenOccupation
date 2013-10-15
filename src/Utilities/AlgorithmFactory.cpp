#include "AlgorithmFactory.hpp"

AlgorithmFactory::~AlgorithmFactory() {
    clear();
}

void AlgorithmFactory::clear() {
    while(!algorithms.empty()) {
        delete algorithms.begin()->second;
        algorithms.erase(algorithms.begin());
    }
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
