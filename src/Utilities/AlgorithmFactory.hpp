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
    /*!
       \brief Destructor.
     */
    ~AlgorithmFactory();

    /*!
       \brief Weather an algorithm with a specific class name has been registered.
     */
    bool has(std::string algorithmClassName);

    /*!
       \brief Get a registered algorithm.
     */
    Algorithm * get(std::string algorithmClassName);

    /*!
       \brief Add/register an algorithm.
     */
    void add(std::string algorithmClassName, Algorithm * algorithm);

    /*!
       \brief Remove all algorithms (delete).
     */
    void clear();
private:
    std::map<std::string, Algorithm*> algorithms;
} ;

#endif
//ALGORITHM_FACTORY_HPP
