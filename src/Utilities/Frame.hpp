#ifndef FRAME_H
#define FRAME_H

#include <opencv2/core/core.hpp>

#include <map>

/*!
 *  \brief     A container of a snap shot of a discrete time step.
 *  \details   Contain raw images from all camreas taken at the same time.
 *             Contain persons seen.
 *             Contain all intermediate debuging data.
 *  \version   0.1
 *  \date      2013-10-07
 */

typedef struct CameraObject
{
    std::string roomID;
    cv::Mat rawImage;

    //debug information
    std::map<std::string,cv::Mat> processHistory;
} CameraObject;

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
