#ifdef HEADLESS
#ifndef MASKING_HPP
#define MASKING_HPP
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <time.h>
#include <stdio.h>
#include <assert.h>

#include <list>
#include <vector>
#include <queue>
#include <deque>
#include <map>
#include <cmath>

#include <random>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>

#include "../DenseKitchen.hpp"


namespace masking{
	void drawPolygonsToMat(cv::Mat targetMat, const cv::Point **polygonPtrPtr, int numberOfPoints[], cv::Scalar color);
	void drawPolygons(std::string maskType, std::vector<std::vector<cv::Point> > polygons, cv::Scalar color);
	void drawCheckPointCircles();
	void drawPolygon(std::vector<cv::Point> polygon, cv::Scalar color);
	void drawPolygonsToMasks();
	bool readMasks(std::vector<std::vector<cv::Point> > &polygons, std::string nodeName);
	void loadMaskFromFile();
	void sendMasksToFrameList(DenseKitchen* mainProgram);
}
#endif
#endif