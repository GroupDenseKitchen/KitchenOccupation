#include "Object.hpp"

Object::Object()
{
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
    hasPassedMasksThree = false;
    hasAlreadyEntered = false;
    lost = false;
    lifeSpan = 1;

    // Kalman init
    kalmanFilter.init(4,2);
    kalmanFilter.transitionMatrix = *(cv::Mat_<float>(4, 4) << 1,0,1,0,
                                                               0,1,0,1,
                                                               0,0,1,0,
                                                               0,0,0,1);
    cv::Mat_<float> measurement(2,1);
    measurement.setTo(cv::Scalar(0));

    // Init
    kalmanFilter.statePre.at<float>(0) = centerOfMass.x;
    kalmanFilter.statePre.at<float>(1) = centerOfMass.y;
    kalmanFilter.statePre.at<float>(2) = 0;
    kalmanFilter.statePre.at<float>(3) = 0;
    setIdentity(kalmanFilter.measurementMatrix);
    setIdentity(kalmanFilter.processNoiseCov, cv::Scalar::all(1e-4));
    setIdentity(kalmanFilter.measurementNoiseCov, cv::Scalar::all(1e-1));
    setIdentity(kalmanFilter.errorCovPost, cv::Scalar::all(.1));
}

Object::~Object()
{

}

void Object::merge(Object * previousState) {
    id = previousState->id;
    lost = previousState->lost;
    entryPoint = previousState->entryPoint;
    lifeSpan = previousState->lifeSpan+1;

    // In/Out detection auxilirary variables.
    if(previousState->hasPassedMasksOne == true)
    hasPassedMasksOne = previousState->hasPassedMasksOne;
    if(previousState->hasPassedMasksTwo == true)
    hasPassedMasksTwo = previousState->hasPassedMasksTwo;
    if(previousState->hasPassedMasksThree == true)
    hasPassedMasksThree = previousState->hasPassedMasksThree;
    hasAlreadyEntered = previousState->hasAlreadyEntered;

    // Temporary hack (remove when kalman filter is operational
    velocity = centerOfMass - previousState->centerOfMass;
    positionPrediction = centerOfMass + velocity;
    velocityPrediction = velocity;


    // Kalman filter prediction
    // ------------------------
    kalmanFilter = previousState->kalmanFilter;

    // Measure
    cv::Mat_<float> measurement(2,1);
    measurement(0) = centerOfMass.x;
    measurement(1) = centerOfMass.y;
    cv::Mat estimated = kalmanFilter.correct(measurement);
    //position = cv::Point(estimated.at<float>(0),estimated.at<float>(1));
    velocity = cv::Point(estimated.at<float>(2),estimated.at<float>(3));

    // Predict
    cv::Mat prediction = kalmanFilter.predict();
    positionPrediction = cv::Point(prediction.at<float>(0),prediction.at<float>(1));
    velocityPrediction = cv::Point(prediction.at<float>(2),prediction.at<float>(3));

    LOG("Kalman", "Position(" << estimated.at<float>(0) << "," << estimated.at<float>(1) << "), Velocity(" <<  estimated.at<float>(2) << "," << estimated.at<float>(3) << ")" );

    /*
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



