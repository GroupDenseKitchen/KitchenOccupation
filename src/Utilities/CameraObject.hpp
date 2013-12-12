#ifndef CAMERA_OBJECT_HPP
#define CAMERA_OBJECT_HPP

#include <opencv2/core/core.hpp>

#include <list>
#include <map>

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"

/*!
 * \brief     A process history entry is a container of image processing history.
 * \details   Stored is a tag and a time associated with a resulting image matrix.
 */
struct ProcessHistoryEntry
{
    std::string tag;
    std::string time;
    cv::Mat image;
    ProcessHistoryEntry(std::string tag, std::string time, cv::Mat image) : tag(tag), time(time), image(image) {}
};

/*!
 * \brief       The CameraObject class represents a snapshot taken from a physical camera or similar sensor.
 * \details     This class contains intermediate steps of the image processing pipeline for this snapshot together with information
 *              about the number of people that has entered or exited in this frame as well as queue information.
 */
class CameraObject
{
public:
    /*!
     * \brief Constructor.
     */
    CameraObject();

    /*!
     * \brief Destructor.
     */
    ~CameraObject();

    /*!
     * \brief       Add a new image to the camera stash.
     * \details     Only one raw image exist, all other are processed images
     *              from different stages in the image processing pipeline.
     * \param tag   Desired name of the image.
     * \param image The image.
     */
    void addImage(std::string tag, cv::Mat image);

    /*!
     * \brief       Query if an image with the tag exist in the camera stash.
     * \param tag   Name of requested image
     * \return      True if the image exists
     */
    bool hasImage(std::string tag);

    /*!
     * \brief     Get an image from the camera stash.
     * \param tag Image name
     * \return    The image.
     */
    cv::Mat getImage(std::string tag);

    /*!
     * \brief   Get a vector of the known objects.
     * \return  A reference to the vector of known objects in the last frame.
     */
    std::vector<Object> & getObjects()          { return objects; }

    /*!
     * \brief   Get a vector of the transitionary objects.
     * \return  A reference to the vector of transitionary objects in the last frame.
     */
    std::vector<Object> & getTransitionaryObjects()         { return transitionaryObjects; }

    /*!
     * \brief   Get a vector of the newly found elevated objects.
     * \return  A reference to the vector of newly found objects in the last frame.
     */
    std::vector<Object> & getNewlyFoundObjects()         { return newlyFoundObjects; }

    /*!
     * \brief   Get a vector of potential objects.
     * \return  A reference to the vector of potential objects in the last frame.
     */
    std::vector<Object> & getPotentialObjects() { return potentialObjects; }

    /*!
     * \brief   Get the map of cameraObject images.
     * \return  A reference to the map containing all images.
     */
    std::map<std::string, cv::Mat> & getImages()  { return images; }

    /*!
     * \brief           Set room ID.
     * \details         Not currently in use since only single-room setups are supported.
     * \param roomID    Desired room ID
     */
    void setRoomID(std::string roomID)          { this->roomID = roomID; }

    /*!
     * \brief           Get room ID.
     * \details         Not currently in use since only single-room setups are supported.
     * \param roomID    Desired room ID
     */
    std::string & getRoomID()                   { return roomID; }

    /*!
     * \brief            Set total entered people.
     * \param newEntered New total number of people entered
     */
    void setEntered(int newEntered) {this->enteredTotal = newEntered; }

    /*!
     * \brief            Set total exited peopple.
     * \param newEntered New total number of people exited
     */
    void setExited(int newExited) {this->exitedTotal = newExited; }

    /*!
     * \brief   Get total number of exited people.
     * \return  The number of people that have exited the room so far.
     */
    int & getExited() {return exitedTotal;}

    /*!
     * \brief   Get total number of entered people.
     * \return  The number of people that have entered the room so far.
     */
    int & getEntered() {return enteredTotal;}

    /*!
     * \brief           Set queue visibility.
     * \param queStatus Status of the current queue
     */
    void setQueVisibility(bool queStatus) { this->queIsVisible = queStatus; }

    /*!
     * \brief   Get queue visibility.
     * \return  True if queue is visible.
     */
    bool getQueVisibility() { return queIsVisible; }

private:
    std::string roomID;
    std::map<std::string,cv::Mat> images;
    std::vector<Object> objects;           // Moving things...
    std::vector<Object> potentialObjects;  // Potentially moving things..
    std::vector<Object> transitionaryObjects; //objects that mabey should be counted as entered or left room
    std::vector<Object> newlyFoundObjects; //elevated objects that has entered the scene
    int enteredTotal;
    int exitedTotal;
    bool queIsVisible; //Whether a que is visible from this camera or not
    int currentQueStatus;


    //debug information
    std::list<ProcessHistoryEntry> processHistory;
};

#endif // CAMERA_OBJECT_HPP
