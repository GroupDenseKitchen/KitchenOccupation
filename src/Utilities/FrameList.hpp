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
     * \brief   TODO
     */
    cv::Mat getExclusionMask() const;

    /*!
     * \brief       Set the exclusion mask
     * \param TODO
     */
    void setExclusionMask(const cv::Mat &value);

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    bool hasExclusionMask();

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    cv::Mat getDoorMask() const;

    /*!
     * \brief       TODO
     * \details     TODO
     * \param value TODO
     */
    void setDoorMask(const cv::Mat &value);

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    bool hasDoorMask();

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    cv::Mat getInclusionMask() const;

    /*!
     * \brief       TODO
     * \details     TODO
     * \param value TODO
     */
    void setInclusionMask(const cv::Mat &value);

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    bool hasInclusionMask();

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    bool hasCheckPointMasks();

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    cv::Mat getCheckPointMaskSmall() const;

    /*!
     * \brief       TODO
     * \details     TODO
     * \param value TODO
     */
    void setCheckPointMaskSmall(const cv::Mat &value);

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    cv::Mat getCheckPointMaskMedium() const;

    /*!
     * \brief       TODO
     * \details     TODO
     * \param value TODO
     */
    void setCheckPointMaskMedium(const cv::Mat &value);

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    cv::Mat getCheckPointMaskLarge() const;

    /*!
     * \brief       TODO
     * \details     TODO
     * \param value TODO
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
