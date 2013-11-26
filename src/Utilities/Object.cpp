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
}

void Object::enter() {
    entryPoint = centerOfMass;
}

void Object::exit() {
    exitPoint = centerOfMass;
    lost = true;
}



