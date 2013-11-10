#ifndef QUE_HPP
#define QUE_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"

struct SplineStrip
{
    SplineStrip(cv::Point2d P0, cv::Point2d C1, cv::Point2d C2, cv::Point2d P1): p0(P0), c1(C1), c2(C2), p1(P1) {}

    double length() {return cv::norm(c1 - p0) + cv::norm(c2 - c1) + cv::norm(p1 - c2);}
    double maxSegmentLength() {return std::max(cv::norm(c1 - p0), std::max(cv::norm(c2 -c1), cv::norm(p1 - c2) ));}

    cv::Point2d p0;
    cv::Point2d c1;
    cv::Point2d c2;
    cv::Point2d p1;
};

struct DirectedQueEdge
{
    DirectedQueEdge(Object fromObj, Object toObj, std::vector<SplineStrip> splineStrips, double dist):
        from(fromObj), to(toObj), spline(splineStrips), distance(dist) {}
    DirectedQueEdge(Object fromObj): from(fromObj), to(fromObj), spline(std::vector<SplineStrip>()), distance( DBL_MAX ) {}

    Object from;
    Object to;
    std::vector<SplineStrip> spline;
    double distance;
};

struct Que
{
public:
    Que();

    std::vector<SplineStrip > splineStrips;
    std::map<int,Object> queObjects; //These are unordered
    std::vector<DirectedQueEdge> queEdges;
};

#endif // QUE_HPP
