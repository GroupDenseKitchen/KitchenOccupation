#include "Debugging/MainDebugWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    MainDebugWindow depugProgram;
    depugProgram.show();

    return a.exec();

}
