#ifndef ALGORITHM_FACTORY_HPP
#define ALGORITHM_FACTORY_HPP

#include "utilities.hpp"
#include "Algorithm.hpp"

/*! 
 *  \brief     Global factory of algorithms
 *  \date      2013-10-15
 *  \version   1.0
 */
class AlgorithmFactory
{
public:
    bool has(std::string algorithmClassName);
    Algorithm * get(std::string algorithmClassName);
    void add(std::string algorithmClassName, Algorithm * algorithm);
    void clear();
private:
    std::map<std::string, Algorithm*> algorithms;
} ;

#endif 
//ALGORITHM_FACTORY_HPP
