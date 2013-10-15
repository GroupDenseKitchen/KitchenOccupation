#ifndef ALGORITHM_FACTORY_HPP
#define ALGORITHM_FACTORY_HPP

#include "utilities.hpp"

class Algorithm;

/*! 
 *  \brief     Global factory of algorithms
 *  \date      2013-10-15
 *  \version   1.0
 */
class AlgorithmFactory
{
public:
    static void initialize();
    static bool has(std::string algorithmClassName);
    static Algorithm * get(std::string algorithmClassName);
    static void add(std::string algorithmClassName, Algorithm * algorithm);
private:
    AlgorithmFactory() {}
    static std::map<std::string, Algorithm*> algorithms;
};

#include "Algorithm.hpp"

#endif 
//ALGORITHM_FACTORY_HPP
