#include "ForegroundRegionExtractorTwo.hpp"

namespace image_processing{

ForegroundRegionExtractorTwo::ForegroundRegionExtractorTwo(){

}

ForegroundRegionExtractorTwo::~ForegroundRegionExtractorTwo(){

}

bool ForegroundRegionExtractorTwo::initialize(configuration::ConfigurationManager &settings){
	return true;
}

cv::Mat ForegroundRegionExtractorTwo::floodFill(cv::Mat img){
	cv::RNG rng = cv::theRNG();
	cv::Mat mask( img.rows+2, img.cols+2, CV_8UC1, cv::Scalar::all(0) );
	cv::Scalar colorDiff=cv::Scalar::all(1);
	cv::Mat res = img.clone();

    CV_Assert( !img.empty() );

    for( int y = 0; y < img.rows; y++ ){
        for( int x = 0; x < img.cols; x++ ){
            if( mask.at<uchar>(y+1, x+1) == 0 ){
                cv::Scalar newVal( rng(256), rng(256), rng(256) );
                cv::floodFill( res, mask, cv::Point(x,y), newVal, 0, colorDiff, colorDiff );
            }
        }
    }
    return res;
}

void ForegroundRegionExtractorTwo::process(FrameList &frames){
	for(CameraObject &cam : frames.getCurrent().getCameras()){
		if(!cam.hasImage("foregroundMask")){
			LOG("Image processing error","Foreground Mask not set in current frame/camera");
			return;
		}
		cv::Mat raw = cam.getImage("rawImage");
		cv::Mat foregroundMask = cam.getImage("foregroundMask");
		//TODO this should be a copy without the background bits

        int spatialRad = 10;
        int colorRad = 10;
        int maxPyrLevel = 1;
        cv::Mat res = raw.clone();
        cv::pyrMeanShiftFiltering(raw,res,spatialRad,colorRad,maxPyrLevel);
        cam.addImage("meanShift",res);
        cv::Mat flooded = floodFill(res);

        cam.addImage("flooded",flooded);
	}
}

}
