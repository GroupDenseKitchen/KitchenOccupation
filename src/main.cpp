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
 *      This software was created as a part of the course Bilder och Grafik (TSBB11) at Linköping University.
 *      The goal of the project was to create a software system for counting people and eventually having the system being used to measure
 *      room usage intensity at the university. The software is designed to be as modular and general as possible in order to allow
 *      for easy replacement of different system components.
 *
 *  \subsection software_architecture Modular Software Architechture
 *      The software has a modular and easily extendible design, where each module is responsible for a specific domain of the system.
 *      The architecture of the system is that of a pipeline of algorithms, where data pertaining to the latest few frames is piped through
 *      each pipeline step by the main program. The order is specified in a configuration file and set in place during program initialisation.
 *      The frame data consists of both raw sensor data and computed data from the different pipeline steps.
 *
 *  \subsection configuration_files Configuration File System
 *      The whole system can be configured by editing the plain text YAML configuration files. Among other things,
 *      it is possible to set what image processing algorithms should be run, in what order to run them and the value of most parameters.
 *      Many components in the Debug GUI, camera settings and important file paths are also set in the configuration files.
 *
 *  \subsection user_interface User Interface
 *      The system includes a debugging and configuration GUI where the results from each of the process steps can be viewed in real time, and one step at a time.
 *      It can also be used to assist in tuning and calibrating the system, and is necessary for proper set up of a new sensor or room.
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
