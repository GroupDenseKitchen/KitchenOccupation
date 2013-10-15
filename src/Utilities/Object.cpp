#include "Object.hpp"

Object::Object(cv::Rect boundingBox)
{
    id = -1;
    this->boundingBox = boundingBox;
    center.x = boundingBox.x + boundingBox.width/2.0;
    center.y = boundingBox.x + boundingBox.width/2.0;
}

Object::~Object()
{

}
