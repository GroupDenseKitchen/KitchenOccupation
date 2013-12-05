#include "Object.hpp"

Object::Object()
{
    kalmanFilter.init(4,2);
}

Object::Object(std::vector<cv::Point>& contour, cv::Rect& boundingBox, cv::Point2f& centerOfMass, double area)
{
    id = -1;
    this->boundingBox = boundingBox;
    this->contour = contour;
    this->centerOfMass = centerOfMass;
    this->area = area;

    entryPoint = cv::Point2d(-1, -1);
    exitPoint = cv::Point2d(-1, -1);

    lost = false;
    lifeSpan = 1;

    // Initialize Kalman filter
    kalmanFilter.init(4,2);

    // State space representation of position and velocity in 2D
    float dt = 1;
    const cv::Matx44f transit( 1, 0, dt, 0,
                               0, 1, 0,  dt,
                               0, 0, 1,  0,
                               0, 0, 0,  1 );

    // More initialization of Kalman filter
    kalmanFilter.statePost.at<float>(0) = centerOfMass.x;
    kalmanFilter.statePost.at<float>(1) = centerOfMass.y;
    kalmanFilter.statePost.at<float>(2) = 0;
    kalmanFilter.statePost.at<float>(3) = 0;
    kalmanFilter.transitionMatrix = cv::Mat(transit);
    cv::setIdentity(kalmanFilter.measurementMatrix);
    cv::setIdentity(kalmanFilter.processNoiseCov, cv::Scalar::all(1e-2)); //faster convergence - more noise
    cv::setIdentity(kalmanFilter.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(kalmanFilter.errorCovPost, cv::Scalar::all(.1));
}

Object::~Object()
{

}

void Object::merge(Object * previousState) {
    id = previousState->id;
    lost = previousState->lost;
    entryPoint = previousState->entryPoint;
    lifeSpan = previousState->lifeSpan+1;

    // Kalman filter prediction
    // ------------------------

    // Initialize known variables
    kalmanFilter = previousState->kalmanFilter;

    // Prediction
    cv::Mat prediction = kalmanFilter.predict();
    cv::Point2f predictedState(prediction.at<float>(0), prediction.at<float>(1));
    positionPrediction = predictedState;
    // Assumption predictedState = (x+dx, y+dy)
    // (x+dx, y+dy) - (x, y) = (dx, dy)
    velocityPrediction = predictedState - previousState->centerOfMass;

    // Correction
    cv::Mat estimated = kalmanFilter.correct(cv::Mat(centerOfMass));
    cv::Point2f state(estimated.at<float>(0), estimated.at<float>(1));
    velocity = state - previousState->centerOfMass;

    // ---------------------
}

void Object::enter() {
    entryPoint = centerOfMass;
}

void Object::exit() {
    exitPoint = centerOfMass;
    lost = true;
}



