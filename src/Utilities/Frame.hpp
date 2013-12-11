#ifndef FRAME_H
#define FRAME_H

#include <opencv2/core/core.hpp>

#include <list>
#include <map>

#include "../Utilities/utilities.hpp"
#include "../Utilities/Object.hpp"
#include "CameraObject.hpp"

/*!
 * \brief   Struct for keeping track of people entering the same room from different cameras.
 */
struct roomPopulation{
    int people;
    std::string roomID;
};

/*!
 * \brief     A container of a snap shot of a discrete time step.
 * \details   Contains raw images from all camreas taken at the same time.
 *            Contains persons seen.
 *            Contains all intermediate debuging data.
 */
class Frame
{
public:

    /*!
     * \brief   Constructor.
     */
    Frame();

    /*!
     * \brief   Destructor.
     */
    ~Frame();

    /*!
     * \brief   Adds a CameraObject to the frame.
     * \param c A CameraObject, representing a physical camera.
     */
    void addCamera(CameraObject c);

    /*!
     * \brief   Returns all camera objects.
     * \return  Reference to a vector of CameraObjects, each representing a physical camera.
     */
    std::vector<CameraObject> &getCameras();

    /*!
     * \brief        Returns a vector of all raw images from all cameras in a room.
     * \param roomID The room ID.
     * \return       Vector with all unprocessed images.
     */
    std::vector<cv::Mat> getRoomImages(std::string roomID);

    /*!
     * \brief   Returns the room IDs for all rooms.
     * \return  A vector containing all room names.
     */
    std::vector<std::string> getRoomIDs();

    /*!
     * \brief   Get the momentary FPS value.
     * \return  1/t where t is the time (in seconds) spent since previous call to Network::dequeFrame.
     */
    double getMomentaryFps() const;

    /*!
     * \brief       Set the momentary FPS variable.
     * \param value New value.
     */
    void setMomentaryFps(double value);

    /*!
     * \brief          Initializes the population of all rooms (sets to zero).
     * \param _cameras A vector of cameraObjects representing all physical sensors.
     */
    void initRoomPopulations(std::vector<CameraObject> &_cameras);

    /*!
     * \brief           Set the room population in a room.
     * \param _newVal   Desired new value
     * \param _currID   ID of the room whose value is to be changed.
     */
    void setPopulationInRoomID(int _newVal, std::string &_currID);

    /*!
     * \brief           Get the number of people in a room.
     * \param _currID   ID of the room.
     * \return          Number of people currently in the room.
     */
    int getPopulationInRoomID(std::string &_currID);

    /*!
     * \brief              Set the the status of "queueness" estimated to this frame.
     * \param newQueStatus Desired "queueness" value.
     */
    void setQueStatus(int newQueStatus);

    /*!
     * \brief   Get the the status of "queueness" estimated to this frame.
     * \return  Current "queueness".
     */
    int getQueStatus();

private:
    std::vector<CameraObject> cameras;

    double momentaryFps;
    std::vector<roomPopulation> populations;

    bool hasID(std::string &_currID);

    int currentQueStatus; 
};

#endif // FRAME_H
