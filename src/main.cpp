#include "Debugging/Logger.hpp"
#include "DenseKitchen.hpp"

#include <QApplication>
#include <QtCore>
#include <iostream>

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainDebugWindow debugProgram;
    //debugProgram.init();
    //debugProgram.show();

   DenseKitchen dk = DenseKitchen(argc,argv);

   return 0;

    //return a.exec();
}
