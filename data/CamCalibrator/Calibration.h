/*  Calibration.h
 *  Header for the the camera calibration module
 *  Written: 2013-07-26
 *  Last edit:
 *
 *  Class Descriprion:
 *  Performs calculation of camera matrices and distortion coefficeients,
 *  as well as undistortion of images used for 3D reconstruction.
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <string>
#include <vector>
#include <list>
#include <iostream>

//#include "utils.h"

class Calibration
{
public:
    Calibration() {}
    // Load configuration file, if unable, load default parameters and create a new config file
    Calibration(std::string configFile);

    // Perform calculation of camera parameters and save to file
    void calibrateCamera(std::string imageFilePath,
                         std::string outputFilePath);
    // Undistort images
    void undistortImages(std::string sourceImage, std::string targetFolder);

    // Create default configuration file
    void createConfigurationFile();

    cv::Size resolution;
    cv::Size patternSize;
    double squareSize;

    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
    double calibrationError;

    std::list<cv::Mat> imageList;
    std::vector<std::vector<cv::Point2f> > patternCoords2D;
    std::vector<std::vector<cv::Point3f> > patternCoords3D;

    // load images
    void loadImages(std::string imagePath, bool color);
    // add detected pattern coords to 2D and 3D point vectors
    void addPatternCoords(cv::Mat image,
                          std::vector<cv::Point2f>& corners,
                          bool refine = true,
                          cv::Size searchWinSize = cv::Size(21,21),
                          cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS,0,0.0001));
    // calculate the actual camera matrix and distortion coefficients
    double calculateCoefficients();
    // save calibration result to file
    void saveParameters(std::string fileName);
    // create the 3D point coordinates of the calibration pattern
    std::vector<cv::Point3f> createCalibrationPoints3D(cv::Size boardSize, double squareSize);
    // look for the calibration pattern in an image
    std::vector<cv::Point2f> findPatternCoords(cv::Mat image, double samplingFactor);

};

#endif // CALIBRATION_H
