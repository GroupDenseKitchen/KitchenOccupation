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
 * \version   0.1
 * \date      2013-10-10
 */
struct ProcessHistoryEntry
{
    std::string tag;
    std::string time;
    cv::Mat image;
    ProcessHistoryEntry(std::string tag, std::string time, cv::Mat image) : tag(tag), time(time), image(image) {}
};

/*!
 * \brief     TODO...
 * \details   TODO...
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
     * \param tag   TODO
     * \param image TODO
     */
    void addImage(std::string tag, cv::Mat image);

    /*!
     * \brief     Weather or not an image with a specific tag exist within the camera stash.
     * \details   TODO
     * \param tag TODO
     * \return    TODO
     */
    bool hasImage(std::string tag);

    /*!
     * \brief     Get an image from the camera stash.
     * \details   TODO
     * \param tag TODO
     * \return    TODO
     */
    cv::Mat getImage(std::string tag);

    /*!
     * \brief   Get a vector of the known objects.
     * \details TODO
     * \return  TODO
     */
    std::vector<Object> & getObjects()          { return objects; }

    /*!
     * \brief   Get a vector of the transitionary objects.
     * \details TODO
     * \return  TODO
     */
    std::vector<Object> & getTransitionaryObjects()         { return transitionaryObjects; }

    /*!
     * \brief   Get a vector of the newly found elevated objects.
     * \details TODO
     * \return  TODO
     */
    std::vector<Object> & getNewlyFoundObjects()         { return newlyFoundObjects; }

    /*!
     * \brief   Get a vector of potential objects.
     * \details TODO
     * \return  TODO
     */
    std::vector<Object> & getPotentialObjects() { return potentialObjects; }

    /*!
     * \brief   Get a map of all images.
     * \details TODO
     * \return  TODO
     */
    std::map<std::string, cv::Mat> & getImages()  { return images; }

    /*!
     * \brief        Set room ID.
     * \details      TODO
     * \param roomID TODO
     */
    void setRoomID(std::string roomID)          { this->roomID = roomID; }

    /*!
     * \brief   Get room ID.
     * \details TODO
     * \return  TODO
     */
    std::string & getRoomID()                   { return roomID; }

    /*!
     * \brief            Set total entered peopple.
     * \details          TODO
     * \param newEntered TODO
     */
    void setEntered(int newEntered) {this->enteredTotal = newEntered; }

    /*!
     * \brief           Set total exited peopple.
     * \details         TODO
     * \param newExited TODO
     */
    void setExited(int newExited) {this->exitedTotal = newExited; }

    /*!
     * \brief   Get exited people.
     * \details TODO
     * \return  TODO
     */
    int & getExited() {return exitedTotal;}

    /*!
     * \brief   Get entered people.
     * \details TODO
     * \return  TODO
     */
    int & getEntered() {return enteredTotal;}

    /*!
     * \brief           Set que visibility.
     * \details         TODO
     * \param queStatus TODO
     */
    void setQueVisibility(bool queStatus) { this->queIsVisible = queStatus; }

    /*!
     * \brief   Get que visibility.
     * \details TODO
     * \return  TODO
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
