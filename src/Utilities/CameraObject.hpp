#ifndef CAMERA_OBJECT_HPP
#define CAMERA_OBJECT_HPP

#include <opencv2/core/core.hpp>

#include <list>
#include <map>

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"


/*!
 *  \brief     A process history entry is a container of image processing history.
 *  \details   Stored is a tag and a time associated with a resulting image matrix.
 *  \version   0.1
 *  \date      2013-10-10
 */
struct ProcessHistoryEntry
{
    std::string tag;
    std::string time;
    cv::Mat image;
    ProcessHistoryEntry(std::string tag, std::string time, cv::Mat image) : tag(tag), time(time), image(image) {}
};


/*!
 *  \brief     TODO...
 *  \details   TODO...
 */
class CameraObject
{
public:
    /*!
       \brief Constructor.
     */
    CameraObject();

    /*!
       \brief Destructor.
     */
    ~CameraObject();

    /*!
       \brief   Add a new image to the camera stash.
       \details Only one raw image exist, all other are processed images
                from different stages in the image processing pipeline.
    */
    void addImage(std::string tag, cv::Mat);

    /*!
       \brief   Weather or not an image with a specific tag exist within the camera stash.
    */
    bool hasImage(std::string tag);

    /*!
       \brief   Get an image from the camera stash.
    */
    cv::Mat getImage(std::string tag);


public:
    std::string roomID;
    std::map<std::string,cv::Mat> images;
    std::vector<Object> objects;  // Moving things...

    //debug information
    std::list<ProcessHistoryEntry> processHistory;
};

#endif // CAMERA_OBJECT_HPP
