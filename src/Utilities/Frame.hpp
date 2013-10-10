#ifndef FRAME_H
#define FRAME_H

#include <opencv2/core/core.hpp>

#include <list>
#include <map>

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"

/*!
 *  \brief     A container of a snap shot of a discrete time step.
 *  \details   Contain raw images from all camreas taken at the same time.
 *             Contain persons seen.
 *             Contain all intermediate debuging data.
 *  \version   0.1
 *  \date      2013-10-07
 */

struct ProcessHistory
{
    std::string tag;
    std::string time;
    cv::Mat image;
    ProcessHistory(std::string tag, std::string time, cv::Mat image) : tag(tag), time(time), image(image) {}
};

/*!
 *  \brief     TODO...
 *  \details   TODO...
 *  \version   0.1
 *  \date      2013-10-10
 */
class CameraObject
{
public:
    CameraObject();
    ~CameraObject();

    /*!
       \brief   TODO...
    */
    void addImage(std::string tag, cv::Mat);

    /*!
       \brief   TODO...
    */
    bool hasImage(std::string tag);

    /*!
       \brief   TODO...
    */
    cv::Mat getImage(std::string tag);


public:
    std::string roomID;
    std::map<std::string,cv::Mat> images;
    std::vector<Object> objects;  // Moving things...

    //debug information
    std::list<ProcessHistory> processHistory;
};

/*!
 *  \brief     A container of a snap shot of a discrete time step.
 *  \details   Contain raw images from all camreas taken at the same time.
 *             Contain persons seen.
 *             Contain all intermediate debuging data.
 *  \version   0.1
 *  \date      2013-10-10
 */
class Frame
{
public:

    friend class MainDebugWindow;

    /*!
       \brief   Constructor.
    */
    Frame();

    /*!
       \brief   Destructor.
    */
    ~Frame();

    /*!
       \brief   TODO...
    */
    void addCamera(CameraObject c);

    /*!
       \brief   TODO...
    */
    std::vector<CameraObject> getCameras();

    /*!
       \brief   TODO...
    */
    std::vector<cv::Mat> getRoomImages(std::string roomID);

    /*!
       \brief   TODO...
    */
    std::vector<std::string> getRoomIDs();


private:
    std::vector<CameraObject> cameras;
};

#endif // FRAME_H
