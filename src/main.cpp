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
	exit(-1);
   }else{
	printf("INIT SUCCESS, NOW RUNNING!!! \n");
   }

   //run the acual program
   bool iterOK = dk.singleIteration();

   if(iterOK){
	printf("successfull iteration, success is now inf! \n");
   }else{
	printf("error iterating! \n");
	exit(-1);
   }


   return 0;
#endif
}
