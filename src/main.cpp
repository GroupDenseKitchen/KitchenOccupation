#include "Debugging/MainDebugWindow.hpp"
#include "Debugging/Logger.hpp"
#include "DenseKitchen.hpp"

#include <QApplication>
#include <QtCore>
#include <iostream>


/*!
 *  \mainpage Kichen Occupation
 *
 *  \section About
 *      This software was created as a part of the course Bilder och Grafik (TSBB11) at linköping university.
 *      The goal of the project was to create a software system for counting people and eventually haveing the system being used to measure
 *      room usage intensity at the university. The software is designed to be as modular and general as possible in order to allow
 *      for easy replacement of different system components.
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDebugWindow debugProgram;
    debugProgram.init("dense_conf.yml", "guiConfig.yml");
    debugProgram.show();

    return a.exec();
}
