#ifndef ALGORITHM_FACTORY_HPP
#define ALGORITHM_FACTORY_HPP

#include "utilities.hpp"
#include "Algorithm.hpp"

/*!
 * \brief     Global factory of algorithms
 * \details   TODO
 */
class AlgorithmFactory
{
public:
    /*!
     * \brief Destructor.
     */
    ~AlgorithmFactory();

    /*!
     * \brief   Queries wether an algorithm with a specific class name has been registered.
     * \param   algorithmClassName The name of the algorithm
     * \return  True if found.
     */
    bool has(std::string algorithmClassName);

    /*!
     * \brief   Get a pointer to a registered algorithm.
     * \param   algorithmClassName The name of registered algorithm.
     * \return  Pointer to requested algorithm.
     */
    Algorithm * get(std::string algorithmClassName);

    /*!
     * \brief   Add/register an algorithm.
     * \details Adds an algorithm with the specified name to this algorithm's list of sub algorithms.
     * \param   algorithmClassName Name of the algorithm to be added.
     * \param   algorithm Pointer to the algorithm to be added.
     */
    void add(std::string algorithmClassName, Algorithm * algorithm);

    /*!
     * \brief   Remove all algorithms and sub algorithms (delete).
     */
    void clear();

private:
    std::map<std::string, Algorithm*> algorithms;
} ;

#endif
//ALGORITHM_FACTORY_HPP
