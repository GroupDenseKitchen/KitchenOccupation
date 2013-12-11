#ifndef QUE_HPP
#define QUE_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"

/*!
 * \brief    Statistical analysis.
 * \details  Estimates the number of people, models, flows etc..
 */
namespace statistics
{

/*!
 * \brief      A cubic bezier spline.
 * \details    This struct holds the four control points of a cubic bezier spline
 *              and can be queried about its (approximate) length and its longest segment.
 */
struct SplineStrip
{
    /*!
     * \brief First on-curve control point.
     */
    cv::Point2f p0;

    /*!
     * \brief First off-curve control point.
     */
    cv::Point2f c1;

    /*!
     * \brief Second off-curve control point.
     */
    cv::Point2f c2;

    /*!
     * \brief Second on-curve control point.
     */
    cv::Point2f p1;

    /*!
     * \brief    Constructor.
     */
    explicit SplineStrip(cv::Point2f P0, cv::Point2f C1, cv::Point2f C2, cv::Point2f P1): p0(P0), c1(C1), c2(C2), p1(P1) {}

    /*!
     * \brief   Calculates upper bound of spline length.
     * \details The control points of a bezier spline define a convex hull for the curve.
     *          The upper bound for the curve is therefore calculated as the lengths of the lines between
     *          each of the control points.
     * \return The lenght.
     */
    float length() {return cv::norm(c1 - p0) + cv::norm(c2 - c1) + cv::norm(p1 - c2);}

    /*!
     * \brief   Calculates th maximum segment length.
     * \details The maximum segment length is the longest distance between consecutive
     *          control points. It can be used to determin when to stop a recursive spline subdivision.
     * \return  Maximum segment length.
     */
    float maxSegmentLength() {return std::max(cv::norm(c1 - p0), std::max(cv::norm(c2 -c1), cv::norm(p1 - c2) ));}
};

/*!
 * \brief   A directed edge between objects in the queue graph.
 * \details The cost/length of an edge is determined by the length of the spline from the
 *          first object/person to the last.
 */
struct DirectedQueEdge
{
    /*!
     * \brief Object that edge starts from.
     */
    Object from;

    /*!
     * \brief Object that edge ends at.
     */
    Object to;

    /*!
     * \brief   The spline that draws the path between them.
     * \details A container holding all the splinestrips resulting from subdivision of
     *          the original spline representation. These are not necessarily in order.
     */
    std::vector<SplineStrip> spline;

    /*!
     * \brief   The distance between the two objects.
     * \details The distance is measured as the length of the connecting spline.
     */
    double distance;

    /*!
     * \brief              Constructor.
     */
    DirectedQueEdge(Object fromObj, Object toObj, std::vector<SplineStrip> splineStrips, float dist):
        from(fromObj), to(toObj), spline(splineStrips), distance(dist) {}

    /*!
     * \brief         Constructor
     */
    DirectedQueEdge(Object fromObj): from(fromObj), to(fromObj), spline(std::vector<SplineStrip>()), distance( DBL_MAX ) {}    
};

/*!
 * \brief   A queue of persons.
 */
struct Que
{
    /*!
     * \brief   All the splinestrips in the queue.
     * \details The splines are subdiveded and ready to draw.
     */
    std::vector<SplineStrip > splineStrips;

    /*!
     * \brief   A map containing all of the objects in the queue.
     * \details The key is the objects id number.
     */
    std::map<int,Object> queObjects;
    
    /*!
     * \brief All the directed edges in the queue.
     */
    std::vector<DirectedQueEdge> queEdges;
};

}
#endif // QUE_HPP
