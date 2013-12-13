#ifndef FRAME_LIST_HPP
#define FRAME_LIST_HPP

#include <deque>

#include "utilities.hpp"
#include "Frame.hpp"

/*!
 * \brief     A container of cronoligicaly ordered Frames.
 * \details   Keeps infinitly or a configurable number of Frames as history apart from the current Frame.
 */
class FrameList
{
public:

    /*!
     * \brief              Constructor.
     * \param framesToKeep The last framesToKeep frames are saved in the FrameList object.
     */
    FrameList(int framesToKeep = 10);

    /*!
     * \brief   Destructor.
     */
    ~FrameList();

    /*!
     * \brief   Get the current (latest) Frame.
     */
    Frame &getCurrent();

    /*!
     * \brief   Get the previous (previously latest) Frame.
     */
    Frame &getPrevious();

    /*!
     * \brief   Weather there are two or more frames.
     */
    bool hasPrevious();

    /*!
     * \brief          Append the FrameList with the latest Frame.
     */
    void append(Frame newFrame);

    /*!
     * \brief   Get the number of frames in history.
     */
    int size() { return (int)frames.size(); }

    /*!
     * \brief   Get the current frame counter.
     */
    int getFrameCount() { return (int)frameCounter; }

    /*!
     * \brief   Returns a matrix the binary exclusion mask.
     */
    cv::Mat getExclusionMask() const;

    /*!
     * \brief       Defines the exclusion mask (an area of the image set to constant zero).
     * \param value A binary mask.
     */
    void setExclusionMask(const cv::Mat &value);

    /*!
     * \brief   Query if an exclusion mask exists.
     * \return  True if exists
     */
    bool hasExclusionMask();

    /*!
     * \brief   Get the doorway.
     * \return  A binary mask defining the doorway.
     */
    cv::Mat getDoorMask() const;

    /*!
     * \brief       Defines the doorway.
     * \param value A binary mask.
     */
    void setDoorMask(const cv::Mat &value);

    /*!
     * \brief   Query if a door mask exists.
     * \return  True if exists
     */
    bool hasDoorMask();

    /*!
     * \brief   Returns the inverse of the exclusion mask
     * \return  The inclusionMask.
     */
    cv::Mat getInclusionMask() const;

    /*!
     * \brief       Sets the inclusion mask.
     * \param value A binary image.
     */
    void setInclusionMask(const cv::Mat &value);

    /*!
     * \brief   Query if an inclusion mask exists.
     * \return  True if an inclusion mask exists.
     */
    bool hasInclusionMask();

    /*!
     * \brief   Query if checkpoint masks are set.
     * \return  True if checkpoint masks exist.
     */
    bool hasCheckPointMasks();

    /*!
     * \brief   Returns the smallest checkpoint mask.
     * \return  A binary image containing the small checkpoint mask.
     */
    cv::Mat getCheckPointMaskSmall() const;

    /*!
     * \brief       Returns the smallest checkpoint mask.
     * \param value A binary image containing the small checkpoint mask.
     */
    void setCheckPointMaskSmall(const cv::Mat &value);

    /*!
     * \brief   Returns the smallest checkpoint mask.
     * \return  A binary image containing the medium checkpoint mask.
     */
    cv::Mat getCheckPointMaskMedium() const;

    /*!
     * \brief   Returns the smallest checkpoint mask.
     * \param   A binary image containing the medium checkpoint mask.
     */
    void setCheckPointMaskMedium(const cv::Mat &value);

    /*!
     * \brief   Returns the smallest checkpoint mask.
     * \return  A binary image containing the large checkpoint mask.
     */
    cv::Mat getCheckPointMaskLarge() const;

    /*!
     * \brief       Returns the smallest checkpoint mask.
     * \param value A binary image containing the large checkpoint mask.
     */
    void setCheckPointMaskLarge(const cv::Mat &value);

private:
    std::deque<Frame> frames;
    cv::Mat doorMask, exclusionMask, inclusionMask, checkPointMaskSmall, checkPointMaskMedium, checkPointMaskLarge;
    bool initiatedDoorMask, initiatedExclusionMask, initiatedCheckPointMasks;

    double smoothFps; // FPS estimate

    unsigned int framesToKeep;
    unsigned int frameCounter;
};

#endif
