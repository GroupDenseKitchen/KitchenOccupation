/*  Calibration.cpp
 *  Written: 2013-07-26
 *  Last edit:
 */


#include "Calibration.h"

using namespace cv;



Calibration::Calibration(std::string configFile)
{

    try
    {
        cv::FileStorage file(configFile, cv::FileStorage::READ);
        patternSize = cv::Size((int)file["chessboardWidth"], (int)file["chessboardHeight"]);
        file["squareSize"] >> squareSize;
        file.release();
    }
    catch (cv::Exception& e)
    {
        std::cout << "Failed to load chessboard pattern settings, using default and creating a new configuration file" << std::endl;
        patternSize = cv::Size(11,8);
        squareSize = 3;
        createConfigurationFile();
    }

}

void Calibration::createConfigurationFile()
{
    cv::FileStorage calibconfig("calibrationConfig.yml", cv::FileStorage::WRITE);
    calibconfig <<  "chessboardWidth" << 11 <<
                    "chessboardHeight" << 8 <<
                    "squareSize" << 3;
    calibconfig.release();
}

void Calibration::calibrateCamera(std::string imageFilePath,
                                  std::string outputFilePath)
{
    std::cout << "Calibrating camera.." << std::endl;
    loadImages(imageFilePath, false);
    resolution = imageList.front().size();
    std::cout << "Image resolution: " << resolution << std::endl;
    for (std::list<cv::Mat>::iterator it = imageList.begin(); it != imageList.end(); it++)
    {
        std::vector<cv::Point2f> corners = findPatternCoords(*it, 0.25);//, 0.5);
        if (!corners.empty())
        {
            addPatternCoords(*it,corners);
        }
    }
    calibrationError = calculateCoefficients();
    saveParameters(outputFilePath);
    imageList.clear();
}


void Calibration::loadImages(std::string imagePath, bool color)
{

    std::string fileType(imagePath.end()-4,imagePath.end());
    std::string currentFile(imagePath.end()-8,imagePath.end()-4);
    std::string path(imagePath.begin(), imagePath.end()-8);

    cv::Mat nextImage;

    int currentImage = atoi(currentFile.c_str());

    nextImage = imread(path + currentFile + fileType, color);
    while(nextImage.data)
    {
        imageList.push_back(nextImage.clone());
        currentImage++;
        currentFile = std::to_string(currentImage);
        while (currentFile.size() < 4)
            currentFile = "0" + currentFile;
        nextImage = imread(path + currentFile + fileType, color);
    }
    std::cout << imageList.size() << " images successfully loaded!" << std::endl;
    return;
}

std::vector<cv::Point2f> Calibration::findPatternCoords(cv::Mat image, double samplingFactor)//, float samplingFactor)
{
    cv::Mat temp;
    std::vector<cv::Point2f> corners, reversedCorners;
    cv::resize(image,temp,cv::Size(0,0),samplingFactor,samplingFactor,CV_INTER_AREA);

    // Find corners in downsampled image
    if(findChessboardCorners(temp, patternSize, corners,
        CV_CALIB_CB_ADAPTIVE_THRESH+CV_CALIB_CB_NORMALIZE_IMAGE+CV_CALIB_CB_FILTER_QUADS+CV_CALIB_CB_FAST_CHECK))
    {
        // Make sure coordinates are in the proper order: upper left first, and bottom right last
        if(corners[0].x < corners.back().x && corners[0].y < corners.back().y)
        {

            for(unsigned int i = 0; i < corners.size(); i++)
            {
                corners[i] = corners[i]*((int)(1/samplingFactor));
            }
            return corners;
        } // If they are not, reverse their order if the coordinates are reasonable
        else if (corners[0].x > corners.back().x && corners[0].y > corners.back().y)
        {
            for(unsigned int i = corners.size(); i > 0; i--)
            {
                reversedCorners.push_back(corners[i-1]*((int)(1/samplingFactor)));
            }
            return reversedCorners;
        }
    }
    corners.clear();
    return corners;
}

void Calibration::addPatternCoords(cv::Mat image,
                                   std::vector<Point2f>& corners,
                                   bool refine,
                                   cv::Size searchWinSize,
                                   cv::TermCriteria criteria)
{
    if (refine)
    {
        cornerSubPix(image,corners,searchWinSize,Size(-1,-1),criteria);
    }
    // Save
    patternCoords2D.push_back(corners);
    patternCoords3D.push_back(createCalibrationPoints3D(patternSize, squareSize));
}

std::vector<cv::Point3f> Calibration::createCalibrationPoints3D(cv::Size boardSize, double squareSize)
{
    std::vector<Point3f> boardCoords;

    for (int i = 0; i < boardSize.height; i++)
    {
        for (int j = 0; j < boardSize.width; j++)
        {
            Point3f boardCoord;
            boardCoord.x = (float)(j*squareSize);
            boardCoord.y = (float)(i*squareSize);
            boardCoord.z = 0;

            boardCoords.push_back(boardCoord);

        }
    }
    return boardCoords;
}

double Calibration::calculateCoefficients()
{
    std::vector<cv::Mat> dummy1, dummy2;
    return cv::calibrateCamera(patternCoords3D, patternCoords2D,
                               resolution, cameraMatrix,
                               distCoeffs, dummy1, dummy2,
                               CV_CALIB_RATIONAL_MODEL);
}


void saveImageList(std::string path, std::list<cv::Mat>* imageList)
{
    char buffer [5];
    string idx;

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    int i = 0;
    for (std::list<cv::Mat>::iterator it = imageList->begin(); it != imageList->end(); it++)
    {
        idx = std::to_string(i);
        while ( idx.size() < 4)
        {
            idx = "0" + idx;
        }

        imwrite(path + "IMG_" + idx + ".PNG", *it, compression_params);
        i++;
    }
}

void Calibration::undistortImages(std::string sourceImage, std::string targetFolder)
{
    std::cout << "Compensating for lens distortion" << std::endl;
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    cv::Mat undistortedImage;
    cv::Mat currentImage = imread(sourceImage);

    std::string fileType(sourceImage.end()-4,sourceImage.end());
    std::string srcIdxString(sourceImage.end()-8,sourceImage.end()-4);
    std::string path(sourceImage.begin(), sourceImage.end()-8);
    std::string dstIdxString;

    int srcIdx = atoi(srcIdxString.c_str());
    int dstIdx = 0;

    while(currentImage.data)
    {
        undistort(currentImage,undistortedImage,cameraMatrix,distCoeffs);
        dstIdxString = std::to_string(dstIdx);
        while (dstIdxString.length() < 4)
        {
            dstIdxString = "0" + dstIdxString;
        }
        imwrite(targetFolder + "IMG_" + dstIdxString + ".PNG", undistortedImage, compression_params);
        dstIdx++;

        srcIdx++;
        srcIdxString = std::to_string(srcIdx);
        while(srcIdxString.length() < 4)
        {
            srcIdxString = "0" + srcIdxString;
        }
        currentImage = imread(path + srcIdxString + fileType);
    }
}


/*
void Calibration::undistortImages(std::string sourceImage, std::string targetFolder)
{
    loadImages(sourceImage, true);
    std::list<cv::Mat> undistortedImages;
    for (std::list<cv::Mat>::iterator it = imageList.begin(); it != imageList.end(); it++)
    {
        cv::Mat image;
        undistort(*it,image,cameraMatrix,distCoeffs);
        undistortedImages.push_back(image);
    }
    saveImageList(targetFolder, &undistortedImages);
}*/

void Calibration::saveParameters(std::string fileName)
{
    FileStorage file(fileName, FileStorage::WRITE);
    file << "CalibrationError" << calibrationError;
    file << "FrameCount" << (int)patternCoords2D.size();
    file << "CameraMatrix" << cameraMatrix
         << "DistCoeffs" << distCoeffs;
    file.release();
}
