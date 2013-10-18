#ifndef FRAME_H
#define FRAME_H

#include <opencv2/core/core.hpp>

#include <list>
#include <map>

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"
#include "CameraObject.hpp"

/*!
 *  \brief     A container of a snap shot of a discrete time step.
 *  \details   Contain raw images from all camreas taken at the same time.
 *             Contain persons seen.
 *             Contain all intermediate debuging data.
 */
class Frame
{
public:

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
    std::vector<CameraObject> &getCameras();

    /*!
       \brief   TODO...
    */
    std::vector<cv::Mat> getRoomImages(std::string roomID);

    /*!
       \brief   TODO...
    */
    std::vector<std::string> getRoomIDs();


public:
    std::vector<CameraObject> cameras;
};

#endif // FRAME_H
