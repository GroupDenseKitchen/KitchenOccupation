#include "Object.hpp"

Object::Object()
{
    kalmanFilter.init(4,4);
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

    // initialize Kalman filter?
    // State space representation of position and velocity in 2D
    kalmanFilter.init(4,2);

    float dt = 1;
    const cv::Matx44f transit( 1, 0, dt, 0,
                               0, 1, 0,  dt,
                               0, 0, 1,  0,
                               0, 0, 0,  1 );
    const cv::Matx21f measure( 0, 0 );

    // Iniitialize kalman filter
    kalmanFilter.statePost.at<float>(0) = centerOfMass.x;
    kalmanFilter.statePost.at<float>(1) = centerOfMass.y;
    kalmanFilter.statePost.at<float>(2) = 0;
    kalmanFilter.statePost.at<float>(3) = 0;
    kalmanFilter.transitionMatrix = cv::Mat(transit);
    //kalmanFilter.measurementMatrix = cv::Mat(measure);
    cv::setIdentity(kalmanFilter.measurementMatrix);
    cv::setIdentity(kalmanFilter.processNoiseCov, cv::Scalar::all(1e-2));
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

    /*
    // Temporary hack (remove when kalman filter is operational
    velocity = centerOfMass - previousState->centerOfMass;
    positionPrediction = centerOfMass + velocity;
    velocityPrediction = velocity;
    */

    // Kalman filter prediction
    // ------------------------

    // Variables
    //cv::Point2f position, predictedPosition;
    //cv::Point2f velocity, predictedVelocity;

    // Initialize known variables
    kalmanFilter = previousState->kalmanFilter;
    // position = cv::Vec2f(centerOfMass.x, centerOfMass.y);

    // Predict to update internal statePre
    cv::Mat prediction = kalmanFilter.predict();
    cv::Point2f predictedPosition(prediction.at<float>(0), prediction.at<float>(1));
    //cv::Point2f predictedVelocity(prediction.at<float>(2), prediction.at<float>(3));
    // antagit att pos = (x+dx, y+dy)
    positionPrediction = predictedPosition;
    //velocityPrediction = predictedVelocity;
    velocityPrediction = predictedPosition - previousState->centerOfMass;

    // Update measurement
    cv::Point2f position(centerOfMass.x, centerOfMass.y);
    centerOfMass = position;

    // Estimation
    cv::Point2f measurements = cv::Point2f(centerOfMass.x,centerOfMass.y);
    cv::Mat estimated = kalmanFilter.correct(cv::Mat(measurements));
    cv::Point2f statePosition(estimated.at<float>(0), estimated.at<float>(1));
    this->velocity = statePosition - previousState->centerOfMass;

    // ---------------------
}

void Object::enter() {
    entryPoint = centerOfMass;
}

void Object::exit() {
    exitPoint = centerOfMass;
    lost = true;
}



