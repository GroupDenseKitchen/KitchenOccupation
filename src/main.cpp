#include "Utilities/utilities.hpp"
#include "Debugging/Logger.hpp"
#include "DenseKitchen.hpp"
#include "Utilities/FrameList.hpp"
#include "Utilities/Frame.hpp"
#include "Utilities/CameraObject.hpp"


#ifndef HEADLESS
  #include "Debugging/MainDebugWindow.hpp"
  #include <QApplication>
  #include <QtCore>
#endif

cv::FileStorage configFile;
std::vector<cv::Point> polygon;
std::vector<std::vector<cv::Point> > doorPolygons;
std::vector<std::vector<cv::Point> > exclusionPolygons;
bool drawAsCircle, isDrawingCircle;
cv::Point circleCenter;
int circleRadius;

bool readMasks(std::vector<std::vector<cv::Point> > &polygons, std::string nodeName)
{
    std::stringstream ss;
    ss << nodeName << "Count";
    std::vector<int> numberOfPolygons;
    configFile[ss.str()] >> numberOfPolygons;
    std::vector<int> node;
    cv::Point point;

    for(unsigned int i = 0; i < numberOfPolygons.size(); i++){
        polygon.clear();

        for(int j = 0; j < numberOfPolygons[i]; j++){
            ss.str("");
            ss << nodeName << i << j;

            configFile[ss.str()] >> node;

            if(node.size() == 2){
                point = cv::Point(node[0], node[1]);
                polygon.push_back(point);
            }
        }

        polygons.push_back(polygon);
        polygon.clear();
    }

    return true;
}

void loadMaskFromFile()
{

    std::string filePath = "masks.yml";
    if(configFile.open(filePath, cv::FileStorage::READ)){
        readMasks(doorPolygons, "doorPolygons");
        readMasks(exclusionPolygons, "exclusionPolygons");
        configFile["circleCenterX"] >> circleCenter.x;
        configFile["circleCenterY"] >> circleCenter.y;
        configFile["circleRadius"] >> circleRadius;
    }
    configFile.release();
    //sendMasksToFrameList();
}

/*
mainProgram->getFrames()->setDoorMask(doorMask);
mainProgram->getFrames()->setExclusionMask(exclusionMask);
mainProgram->getFrames()->setCheckPointMaskSmall(checkpointMaskSmall);
mainProgram->getFrames()->setCheckPointMaskMedium(checkpointMaskMedium);
mainProgram->getFrames()->setCheckPointMaskLarge(checkpointMaskLarge);
*/

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
   
   bool iterOK = true;

   	//run the acual program
   	iterOK = dk.singleIteration();
	FrameList* frames = dk.getFrames();
	cv::Mat fakeMask;
	if(frames->getCurrent().getCameras().size() > 0){

		fakeMask = frames->getCurrent().
				   getCameras()[0].
				   getImage("rawImage").clone();
		fakeMask.setTo(cv::Scalar(0,0,0));
	}	

	//checkpoint masks
	frames->setCheckPointMaskSmall(fakeMask.clone());
	frames->setCheckPointMaskMedium(fakeMask.clone());
	frames->setCheckPointMaskLarge(fakeMask.clone());	
   	frames->setInclusionMask(fakeMask.clone());	
	frames->setDoorMask(fakeMask.clone());	
	frames->setExclusionMask(fakeMask.clone());

   while(iterOK){
	iterOK = dk.singleIteration();
	if(iterOK){
		printf("iteration success! \n");
   	}else{
		printf("error iterating, dying! \n");
		exit(-1);
   	}
  }


   return 0;
#endif
}
