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
 * \details TODO
 */
struct roomPopulation{
    int people;
    std::string roomID;
};

/*!
 * \brief     A container of a snap shot of a discrete time step.
 * \details   Contain raw images from all camreas taken at the same time.
 *            Contain persons seen.
 *            Contain all intermediate debuging data.
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
     * \details TODO
     * \param c A CameraObject, representing a physical camera.
     */
    void addCamera(CameraObject c);

    /*!
     * \brief   Returns all camera objects.
     * \details TODO
     * \return  A vector of CameraObjects, each representing a physical camera.
     */
    std::vector<CameraObject> &getCameras();

    /*!
     * \brief        Returns a vector of all raw images from all cameras in a room.
     * \details      TODO
     * \param roomID The room.
     * \return       Vector with all unprocessed images.
     */
    std::vector<cv::Mat> getRoomImages(std::string roomID);

    /*!
     * \brief   Returns the room IDs for all rooms.
     * \details TODO
     * \return  A vector containing room names.
     */
    std::vector<std::string> getRoomIDs();

    /*!
     * \brief   Get the momentary FPS value.
     * \details TODO
     * \return  1/t where is the time spent since previous call to Network::dequeFrame.
     */
    double getMomentaryFps() const;

    /*!
     * \brief       Set the momentary FPS variable.
     * \details     TODO
     * \param value New value.
     */
    void setMomentaryFps(double value);

    /*!
     * \brief          Initialize the room populations.
     * \detials        TODO
     * \param _cameras TODO
     */
    void initRoomPopulations(std::vector<CameraObject> &_cameras);

    /*!
     * \brief         Set the room population in a room.
     * \details       TODO
     * \param _newVal TODO
     * \param _currID TODO
     */
    void setPopulationInRoomID(int _newVal, std::string &_currID);

    /*!
     * \brief         Initialize the room population in a room.
     * \details       TODO
     * \param _currID TODO
     * \return        TODO
     */
    int getPopulationInRoomID(std::string &_currID);

    /*!
     * \brief              Set the the status of "queueness" estimated to this frame.
     * \details            TODO
     * \param newQueStatus TODO
     */
    void setQueStatus(int newQueStatus);

    /*!
     * \brief   Get the the status of "queueness" estimated to this frame.
     * \details TODO
     * \return  TODO
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
