#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <opencv2/core/core.hpp>

#include <list>
#include <map>

#include "../Utilities/utilities.hpp"

/*!
 *  \brief     TODO...
 *  \details   TODO...
 *  \version   0.1
 *  \date      2013-10-10
 */
class Object
{
public:
    Object();
    ~Object();

public:
    int id;
    cv::Rect boundingBox;
};

#endif // OBJECT_HPP
