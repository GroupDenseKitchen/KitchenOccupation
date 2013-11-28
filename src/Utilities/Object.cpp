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
    hasPassedMasksOne = false;
    hasPassedMasksTwo = false;
    hasAlreadyEntered = false;
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

    if(previousState->hasPassedMasksOne == true)
    hasPassedMasksOne = previousState->hasPassedMasksOne;
    if(previousState->hasPassedMasksTwo == true)
    hasPassedMasksTwo = previousState->hasPassedMasksTwo;

    hasAlreadyEntered = previousState->hasAlreadyEntered;


    /*
    // Kalman filter prediction
    cv::Vec2f velocity, predictedVelocity;
    float dt = 1;
    const cv::Matx44f transit( 1, 0, dt, 0,
                               0, 1, 0,  dt,
                               0, 0, 1,  0,
                               0, 0, 0,  1 );
    const cv::Matx44f measure( 1, 1, 0, 0  );

    kalmanFilter.transitionMatrix = cv::Mat(transit);
    kalmanFilter.measurementMatrix = cv::Mat(measure);

    velocity = kalmanFilter.correct(cv::Mat(cv::Mat2f( centerOfMass.x, centerOfMass.y)));
    predictedVelocity = kalmanFilter.predict();

    //kalmanFilter.statePre = previous.centerOfMass;
    //kalmanFilter.errorCovPre = cv::Matx22f(1,0,1,0);
    predictedVelocity = kalmanFilter.predict();
    */
}

void Object::enter() {
    entryPoint = centerOfMass;
}

void Object::exit() {
    exitPoint = centerOfMass;
    lost = true;
}



