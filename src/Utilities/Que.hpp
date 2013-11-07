#ifndef QUE_HPP
#define QUE_HPP

#include "../Utilities/utilities.hpp"

struct SplineStrip
{
    SplineStrip(cv::Point2d P0, cv::Point2d C1, cv::Point2d C2, cv::Point2d P1): p0(P0), c1(C1), c2(C2), p1(P1) {}

    cv::Point2d p0;
    cv::Point2d c1;
    cv::Point2d c2;
    cv::Point2d p1;
};

struct Que
{
public:
    Que();

    std::vector<SplineStrip > splineStrips;
};

#endif // QUE_HPP
