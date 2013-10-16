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

    /*!
       \brief   Merge current state of an object with the previous.
    */
    void merge(Object* previousState);

    /*!
       \brief   Called when an object has entered the view.
    */
    void enter();

    /*!
       \brief   Called when an object has exited the view.
    */
    void exit();


public:
    int id;
    cv::Rect boundingBox;
    cv::Point2d center;

    cv::Point2d entryPoint;
    cv::Point2d exitPoint;

    // Status
    bool lost;
};

#endif // OBJECT_HPP
