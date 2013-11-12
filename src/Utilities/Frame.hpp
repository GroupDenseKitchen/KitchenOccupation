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
     * \brief Adds a CameraObject to the frame.
     * \param c A CameraObject, representing a physical camera.
     */
    void addCamera(CameraObject c);

    /*!
     * \brief Returns all camera objects.
     * \return A vector of CameraObjects, each representing a physical camera.
     */
    std::vector<CameraObject> &getCameras();

    /*!
     * \brief Returns a vector of all raw images from all cameras in a room
     * \param roomID The room
     * \return Vector with all unprocessed images.
     */
    std::vector<cv::Mat> getRoomImages(std::string roomID);

    /*!
     * \brief Returns the room IDs for all rooms.
     * \return A vector containing room names.
     */
    std::vector<std::string> getRoomIDs();

    /*!
     * \brief Get the momentary FPS value.
     * \return 1/t where is the time spent since previous call to Network::dequeFrame.
     */
    double getMomentaryFps() const;

    /*!
     * \brief Set the momentary FPS variable.
     * \param value New value;
     */
    void setMomentaryFps(double value);

private:
    std::vector<CameraObject> cameras;

    double momentaryFps;
};

#endif // FRAME_H
