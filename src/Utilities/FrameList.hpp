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
     * \param framesToKeep TODO
     */
    FrameList(int framesToKeep = 10);

    /*!
     * \brief   Destructor.
     */
    ~FrameList();

    /*!
     * \brief   Get the current (latest) Frame.
     * \details TODO
     * \return  TODO
     */
    Frame &getCurrent();

    /*!
     * \brief   Get the previous (previously latest) Frame.
     * \details TODO
     * \return  TODO
     */
    Frame &getPrevious();

    /*!
     * \brief   Weather there are two or more frames.
     * \details TODO
     * \return  TODO
     */
    bool hasPrevious();

    /*!
     * \brief          Append the FrameList with the latest Frame.
     * \details        TODO
     * \param newFrame TODO
     */
    void append(Frame newFrame);

    /*!
     * \brief   Get the number of frames in history.
     * \details TODO
     * \return  TODO
     */
    int size() { return (int)frames.size(); }

    /*!
     * \brief   Get the current frame counter.
     * \details TODO
     * \return  TODO
     */
    int getFrameCount() { return (int)frameCounter; }

    /*!
     * \brief   TODO
     * \details TODO
     * \return  TODO
     */
    cv::Mat getExclusionMask() const;

    /*!
     * \brief       TODO
     * \details     TODO
     * \param value TODO
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
     * \detials TODO
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
