#ifndef ALGORITHM_FACTORY_HPP
#define ALGORITHM_FACTORY_HPP

#include "utilities.hpp"
#include "Algorithm.hpp"

/*!
 * \brief     Global factory of algorithms
 * \details   TODO
 * \date      2013-10-15
 * \version   1.0
 */
class AlgorithmFactory
{
public:
    /*!
     * \brief Destructor.
     */
    ~AlgorithmFactory();

    /*!
     * \brief   Weather an algorithm with a specific class name has been registered.
     * \details TODO
     * \param   algorithmClassName TODO
     * \return  TODO
     */
    bool has(std::string algorithmClassName);

    /*!
     * \brief   Get a registered algorithm.
     * \details TODO
     * \param   algorithmClassName TODO
     * \return  TODO
     */
    Algorithm * get(std::string algorithmClassName);

    /*!
     * \brief   Add/register an algorithm.
     * \details TODO
     * \param   algorithmClassName TODO
     * \param   algorithm TODO
     */
    void add(std::string algorithmClassName, Algorithm * algorithm);

    /*!
     * \brief   Remove all algorithms (delete).
     * \details TODO
     */
    void clear();

private:
    std::map<std::string, Algorithm*> algorithms;
} ;

#endif
//ALGORITHM_FACTORY_HPP
