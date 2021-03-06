#ifndef TIMER_HPP
#define TIMER_HPP

#include "../Utilities/utilities.hpp"

#define TICKS_PER_SECOND        1.0/cv::getTickFrequency();
#define TICKS_PER_MILLISECOND   1e3*1.0/cv::getTickFrequency();
#define TICKS_PER_MICROSECOND   1e6*1.0/cv::getTickFrequency();
#define TICKS_PER_NANOSECOND    1e9*1.0/cv::getTickFrequency();

/*!
 * \brief     Timer that measures time durations from latest reset.
 * \details   Derive form this class to maintain a simple interface for all algorithms used in the processing pipeline.
 */
class Timer
{
public:
    /*!
     * \brief   Constructor, resets the timer.
     */
    Timer()             { reset(); }

    /*!
     * \brief   Resets timer, starts to mesure the time from now.
     */
    void reset()        { begun = cv::getTickCount(); }

    /*!
     * \brief   Reset timer from last sampling (using any get method).
     */
    void resetFromLast() { begun = last; }

    /*!
     * \brief   Get the time in seconds since last reset.
     */
    double getSeconds();

    /*!
     * \brief   Get the time in milliseconds since last reset.
     */
    double getMilliseconds();

    /*!
     * \brief   Get the time in microseconds since last reset.
     */
    double getMicroseconds();

    /*!
     * \brief   Get the time in nanoseconds since last reset.
     */
    double getNanoseconds();

private:
    int64 begun, last;
};

#endif
