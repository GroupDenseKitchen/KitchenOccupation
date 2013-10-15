#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../Utilities/utilities.hpp"

/*!
 *  \brief     A movable object seen (candidate for human)
 *  \version   0.1
 *  \date      2013-10-10
 */
class Object
{
public:

    /*!
       \brief   Constructor.
    */
    Object(cv::Rect boundingBox);

    /*!
       \brief   Destructor.
    */
    ~Object();

public:
    int id;
    cv::Rect boundingBox;
    cv::Point2d center;
};

#endif // OBJECT_HPP
