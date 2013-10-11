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
    //PROFILER_DUMP();
    p.init();
    p.singleIteration();
    PROFILER_DUMP();
    p.singleIteration();
    PROFILER_DUMP();
    return 0;

    //return a.exec();
}
