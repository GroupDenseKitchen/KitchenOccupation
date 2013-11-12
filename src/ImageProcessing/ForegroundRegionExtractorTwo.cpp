#include "ForegroundRegionExtractorTwo.hpp"

namespace image_processing{

ForegroundRegionExtractorTwo::ForegroundRegionExtractorTwo(){

}

ForegroundRegionExtractorTwo::~ForegroundRegionExtractorTwo(){

}

bool ForegroundRegionExtractorTwo::initialize(configuration::ConfigurationManager &settings){
	return true;
}

void ForegroundRegionExtractorTwo::process(FrameList &frames){
	for(CameraObject &cam : frames.getCurrent().getCameras()){
		if(!cam.hasImage("foregroundMask")){
			LOG("Image processing error","Foreground Mask not set in current frame/camera");
			return;
		}

		cv::Mat raw = cam.getImage("rawImage");
		cv::Mat foregroundMask = cam.getImage("foregroundMask");
		cv::Mat roi;
		raw.copyTo(roi,foregroundMask);
		cam.addImage("roi",roi);
	}
}

}