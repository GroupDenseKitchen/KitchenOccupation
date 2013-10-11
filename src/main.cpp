#include "Debugging/MainDebugWindow.hpp"
#include "Debugging/Logger.hpp"
#include "DenseKitchen.hpp"

#include <QApplication>
#include <QtCore>
#include <iostream>

int main(int argc, char *argv[])
{
    /*
    QApplication a(argc, argv);
    MainDebugWindow debugProgram;
    debugProgram.init();
    debugProgram.show();*/

    DenseKitchen p;
    p.readConfig("../conf/dense_conf.yml");
    if(!p.init())
    {
        std::cout << "Init failed!\n";
        return 0;
    }
    p.singleIteration();
    PROFILER_DUMP();
    return 0;

    //return a.exec();
}
