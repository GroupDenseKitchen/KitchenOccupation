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
}

Object::~Object()
{

}

void Object::merge(Object * previousState) {
    id = previousState->id;
    lost = previousState->lost;
    entryPoint = previousState->entryPoint;
    lifeSpan = previousState->lifeSpan+1;

    // Temporary hack (remove when kalman filter is operational
    velocity = centerOfMass - previousState->centerOfMass;
    positionPrediction = centerOfMass + velocity;
    velocityPrediction = velocity;

    /*
    // Kalman filter prediction
    // ------------------------

    // Variables
    cv::Vec2f position, predictedPosition;
    cv::Vec2f velocity, predictedVelocity;

    // Initialize known variables
    kalmanFilter = previousState->kalmanFilter;
    position = cv::Vec2f(centerOfMass.x, centerOfMass.y);

    // State space representation of position and velocity in 2D
    float dt = 1;
    const cv::Matx44f transit( 1, 0, dt, 0,
                               0, 1, 0,  dt,
                               0, 0, 1,  0,
                               0, 0, 0,  1 );
    const cv::Matx44f measure( 1, 1, 0, 0  );

    // Iniitialize kalman filter
    kalmanFilter.transitionMatrix = cv::Mat(transit);
    kalmanFilter.measurementMatrix = cv::Mat(measure);
    // .... more initialization ....

    // Measurement update
    //?? = kalmanFilter.correct(cv::Mat(position));
    //velocity = ??

    // Prediction
    //?? = kalmanFilter.predict();
    //predictedVelocity = ??
    //predictedPosition = ??

    // Convert back to points
    //this->velocity = cv::Point2f(velocity.x, velocity.y);
    //this->predictedPosition = cv::Point2f(predictedPosition.x, predictedPosition.y);
    //this->predictedVelocity = cv::Point2f(predictedVelocity.x, predictedVelocity.y);

    // ---------------------

    */
}

void Object::enter() {
    entryPoint = centerOfMass;
}

void Object::exit() {
    exitPoint = centerOfMass;
    lost = true;
}



