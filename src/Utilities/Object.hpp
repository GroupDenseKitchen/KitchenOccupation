#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/Timer.hpp"

/*!
 *  \brief     A movable object seen (candidate for human)
 */
struct Object
{
    /*!
       \brief   Empty constructor.
    */
    Object();

    /*!
       \brief   Constructor ising a cv::Rect for initialization.
    */
    Object(std::vector<cv::Point> & contour, cv::Rect & boundingBox, cv::Point2f & centerOfMass, double area);

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
    std::vector<cv::Point> contour;
    cv::Point2f centerOfMass;
    double area;

    cv::Point2d entryPoint;
    cv::Point2d exitPoint;

    // Status
    bool lost;
    bool hasPassedMasksOne;
    bool hasPassedMasksTwo;
    bool hasPassedMasksThree;
    bool hasAlreadyEntered;
    int lifeSpan;

    // Prediction
    cv::KalmanFilter kalmanFilter;
};

#endif // OBJECT_HPP
