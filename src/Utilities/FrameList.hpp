#ifndef FRAME_LIST_HPP
#define FRAME_LIST_HPP

#include <deque>

#include "utilities.hpp"
#include "Frame.hpp"

/*!
 *  \brief     A container of cronoligicaly ordered Frames.
 *  \details   Keeps infinitly or a configurable number of Frames as history apart from the current Frame.
 */
class FrameList
{
public:

    /*!
       \brief   Constructor.
    */
    FrameList(int framesToKeep = -1);

    /*!
       \brief   Destructor.
    */
    ~FrameList();

    /*!
       \brief   Get the current (latest) Frame.
    */
    Frame &getCurrent();

    /*!
       \brief   Get the previous (previously latest) Frame.
    */
    Frame &getPrevious();

    /*!
       \brief   Weather there are two or more frames.
    */
    bool hasPrevious();

    /*!
       \brief   Append the FrameList with the latest Frame.
    */
    void append(Frame newFrame);

    /*!
       \brief   Get the number of frames in history.
    */
    int size() { return frames.size(); }
    /*!
       \brief   Get the current frame counter.
    */
    int getFrameCount() { return frameCounter; }

    /*!
     * \brief getExclusionMask
     */
    cv::Mat getExclusionMask() const;

    /*!
     * \brief setExclusionMask
     */
    void setExclusionMask(const cv::Mat &value);

    /*!
     * \brief hasExclusionMask
     */
    bool hasExclusionMask();

    /*!
     * \brief getDoorMask
     */
    cv::Mat getDoorMask() const;

    /*!
     * \brief setDoorMask
     */
    void setDoorMask(const cv::Mat &value);

    /*!
     * \brief hasDoorMask
     */
    bool hasDoorMask();

    /*!
     * \brief getInclusionMask
     */
    cv::Mat getInclusionMask() const;

    /*!
     * \brief setInclusionMask
     */
    void setInclusionMask(const cv::Mat &value);

    /*!
     * \brief hasInclusionMask
     */
    bool hasInclusionMask();

private:
    std::deque<Frame> frames;
    cv::Mat doorMask, exclusionMask, inclusionMask;
    bool initiatedDoorMask, initiatedExclusionMask;

    unsigned int framesToKeep;
    unsigned int frameCounter;
};

#endif
