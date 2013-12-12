#ifndef ALGORITHM_FACTORY_HPP
#define ALGORITHM_FACTORY_HPP

#include "utilities.hpp"
#include "Algorithm.hpp"

/*!
 * \brief     Algorithm Factory store available algorithms that can be added to the pipeline from config file.
 * \warning   Algorithms stored are currently shared, meaning that there exist only ONE algorithm of each type with the same name.\n
 *            This should be improved with cloning of algorithms, since algorithms can contain member data which should not be shared between different instances in the pipeline of the same algorithm type.
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
