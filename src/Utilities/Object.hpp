#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/Timer.hpp"

/*!
 * \brief     A movable object seen (candidate for human).
 * \details   TODO
 */
struct Object
{
    /*!
     * \brief   Empty constructor.
     */
    Object();

    /*!
     * \brief              Constructor using a cv::Rect for initialization.
     * \param contour      TODO
     * \param boundingBox  TODO
     * \param centerOfMass TODO
     * \param area         TODO
     */
    Object(std::vector<cv::Point> & contour, cv::Rect & boundingBox, cv::Point2f & centerOfMass, double area);

    /*!
     * \brief   Destructor.
     */
    ~Object();

    /*!
     * \brief               Merge current state of an object with the previous.
     * \details             TODO
     * \param previousState TODO
     */
    void merge(Object* previousState);

    /*!
     * \brief   Called when an object has entered the view.
     * \details TODO
     */
    void enter();

    /*!
     * \brief   Called when an object has exited the view.
     * \details TODO
     */
    void exit();

public:
    int id;
    cv::Rect boundingBox;
    std::vector<cv::Point> contour;
    double area;

    cv::Point2f centerOfMass;   // Position
    cv::Point2f velocity;       // Velocity (from the kalman filter)
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
