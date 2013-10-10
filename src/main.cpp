#include "DenseKitchen.hpp"
#include <QtCore>
#include "Debugging/Logger.hpp"
#include <iostream>

int main(){


    DenseKitchen program;
    program.readConfig("settings.yml");
    program.init();


    cv::namedWindow("Da window");
    cv::Mat image;

    bool hasNewFrame = program.singleIteration();
    while (hasNewFrame) {

        image =  program.frames.getCurrent().getCameras().back().rawImage;
        cv::imshow("Da window", image);
        cv::waitKey(1); // 20fps;

        hasNewFrame = program.singleIteration();
    }
    debugging::logObject.dumpToConsole();
    cv::waitKey(0);
    return 0;
}
