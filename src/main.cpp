#include "Debugging/Logger.hpp"
#include "DenseKitchen.hpp"

#ifndef HEADLESS
  #include "Debugging/MainDebugWindow.hpp"
  #include <QApplication>
  #include <QtCore>
#endif

#include <iostream>

int main(int argc, char *argv[])
{

#ifndef HEADLESS
    QApplication a(argc, argv);
    MainDebugWindow debugProgram;
    debugProgram.init("dense_conf.yml", "guiConfig.yml");
    debugProgram.show();

    return a.exec();
#else    
   DenseKitchen dk = DenseKitchen();
   bool ok = dk.initialize("dense_conf.yml");
   if(ok == false){
	printf("error initing ! \n");
	exit(0);
   }

   return 0;
#endif
}
