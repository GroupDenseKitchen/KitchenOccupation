#ifndef INCLUDE_H
#define INCLUDE_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <time.h>
#include <stdio.h>
#include <assert.h>

#include <list>
#include <vector>
#include <queue>
#include <deque>
#include <map>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>

#include "../Debugging/Logger.hpp"


#ifdef __linux__
namespace std{
        template<typename Type>
        string to_string(Type v){
                stringstream ss;
                ss << v;
                return ss.str();
        }
}
#endif

#endif
