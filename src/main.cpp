#include "DenseKitchen.hpp"

#include <QtCore>
#include "Debugging/Logger.hpp"
#include <iostream>


int main()
{
    DenseKitchen program;
    program.readConfig("../conf/dense_conf.yml");

    if (!program.init()) {
        std::cout << "Error initializing program" << std::endl;
        debugging::logObject.dumpToConsole();
        return 0;
    }

    cv::namedWindow("camera1.mov");
    cv::Mat image;



    bool hasNewFrame = program.singleIteration();
    while (hasNewFrame) {

        image =  program.frames.getCurrent().getCameras().back().rawImage;
        cv::imshow("camera1.mov", image);
        cv::waitKey(40); // 20fps;

        hasNewFrame = program.singleIteration();
    }
    debugging::logObject.dumpToConsole();
    cv::waitKey(0);
    return 0;

}
