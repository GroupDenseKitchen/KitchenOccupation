#include "DenseKitchen.hpp"
#include <QtCore>
#include "Debugging/Logger.hpp"
#include <iostream>

int main(){

    DenseKitchen program;
    program.init();
    program.readConfig("settings.yml");
    debugging::logObject.dumpToConsole();
    return 0;
}
