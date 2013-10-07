#include "DenseKitchen.hpp"
#include <QtCore>
#include "Debugging/Logger.hpp"
#include <iostream>

int main(){
/*
    cv::FileStorage cfgFile("settings.yml", cv::FileStorage::WRITE);

    //bool asdf = true;
    bool fgsfds = false;
    cv::Mat asdf = cv::Mat::eye(4,4,CV_64FC1);
    cfgFile << "isDebug" << asdf ;
    cfgFile << "isTesting" << fgsfds;
    cfgFile << "asdf" << "fgsfds";
    cfgFile.release();
*/
    DenseKitchen program;
    program.init();
    program.readConfig("settings.yml");
    return 0;
}
