#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/Timer.hpp"

/*!
 * \brief     A movable object that has been detected, and that potentially might be a human.
 */
struct Object
{
    /*!
     * \brief   Constructor.
     */
    Object();

    /*!
     * \brief   Destructor.
     */
    ~Object();

    /*!
     * \brief              Constructor using a cv::Rect for initialization.
     * \param contour      The 2-dimensional contour of the object.
     * \param boundingBox  An axis-aligned bounding box.
     * \param centerOfMass The center of mass of the object contour
     * \param area         Object contour area.
     */
    Object(std::vector<cv::Point> & contour, cv::Rect & boundingBox, cv::Point2f & centerOfMass, double area);

    /*!
     * \brief                   Merge current state of an object with the previous.
     * \details                 First all variables of the previous are copied to this object, including the kalman filter object.
     *                          Then the kalman filter perform a measurement update, estimating the current velocity, and a prediction of the next position and velocity.
     */
    void merge(Object* previousState);

    /*!
     * \brief   Called when an object has entered the view.
     * \details Sets the Object property entryPoint to the current center of mass, thus saving information about
     *          where the object spawned for the first time.
     */
    void enter();

    /*!
     * \brief   Called when an object has exited the view.
     * \details Sets the Object property exitPoint to the to the point were the object was last seen,
     *          thus saving information about where the object exited the field of view.
     */
    void exit();

public:
    int id;
    cv::Rect boundingBox;
    std::vector<cv::Point> contour;
    double area;

    cv::Point2f centerOfMass;           // Position
    cv::Point2f velocity;               // Velocity (from the kalman filter)
    cv::Point2f positionPrediction;     // Predicteed Position
    cv::Point2f velocityPrediction;     // Predicted Velocity

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
