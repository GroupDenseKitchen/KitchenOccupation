#include "Timer.hpp"

double Timer::getSeconds() {
    last = cv::getTickCount();
    return double(last - begun)*TICKS_PER_SECOND;
}

double Timer::getMilliseconds() {
    last = cv::getTickCount();
    return double(last - begun)*TICKS_PER_MILLISECOND;
}

double Timer::getMicroseconds() {
    last = cv::getTickCount();
    return double(last - begun)*TICKS_PER_MICROSECOND;
}

double Timer::getNanoseconds() {
    last = cv::getTickCount();
    return double(last - begun)*TICKS_PER_NANOSECOND;
}
