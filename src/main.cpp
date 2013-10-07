#include "Debugging/MainDebugWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    MainDebugWindow debugProgram;
    debugProgram.init();
    debugProgram.show();

    return a.exec();

}
