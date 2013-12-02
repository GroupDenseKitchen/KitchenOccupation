#ifndef QUE_HPP
#define QUE_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"

struct SplineStrip
{
    SplineStrip(cv::Point2f P0, cv::Point2f C1, cv::Point2f C2, cv::Point2f P1): p0(P0), c1(C1), c2(C2), p1(P1) {}

    float length() {return cv::norm(c1 - p0) + cv::norm(c2 - c1) + cv::norm(p1 - c2);}
    float maxSegmentLength() {return std::max(cv::norm(c1 - p0), std::max(cv::norm(c2 -c1), cv::norm(p1 - c2) ));}

    cv::Point2f p0;
    cv::Point2f c1;
    cv::Point2f c2;
    cv::Point2f p1;
};

struct DirectedQueEdge
{
    DirectedQueEdge(Object fromObj, Object toObj, std::vector<SplineStrip> splineStrips, float dist):
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
    //std::map<int,Object> queObjects; //These are unordered (and so far unused)
    std::vector<DirectedQueEdge> queEdges;
};

#endif // QUE_HPP
