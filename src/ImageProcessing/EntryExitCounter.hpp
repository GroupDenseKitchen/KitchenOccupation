#ifndef ENTRYEXITCOUNTER_HPP
#define ENTRYEXITCOUNTER_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"


namespace image_processing
{
/*!
 *  \brief     Process step which determines if objects are lost in an entry area, creates bounding boxes.
 */
class EntryExitCounter : public Algorithm
{
public:

    /*!
       \brief   Constructor.
    */
    EntryExitCounter();

    /*!
       \brief   Destructor.
    */
    ~EntryExitCounter();

    /*!
       \brief   Performs the process step.
    */
    void process(FrameList &frames) override;
    /*!
       \brief   Initialize the algorithm.
       \details Returns false if initialization fails,
                e.g. if a required variable is not set
    */
     bool initialize(configuration::ConfigurationManager &conf) override;

     /*!
        \brief   Checks if point is inside a polygon.
     */

     /*!
        \brief   Checks if a 2D-point is inside a polygon.
     */
     static bool isInsidePolygon(cv::Mat mask, cv::Point2d point){
         if(point.x >= 0 && point.y >= 0){
             if(mask.at<cv::Vec3b>(point)[0] == 255){
                 return true;
             }
         }

         return false;
     }

private:

};
}
#endif
