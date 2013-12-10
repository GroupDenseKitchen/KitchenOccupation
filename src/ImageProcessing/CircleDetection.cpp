#include "CircleDetection.hpp"

#include <QDebug>

namespace image_processing
{

    CircleDetection::CircleDetection()
    {
    }

    bool CircleDetection::initialize(configuration::ConfigurationManager &settings)
    {
        CONFIG(settings, lowThreshold, "lowThreshold", 100);
        CONFIG(settings, highThreshold, "highThreshold", 200);
        CONFIG(settings, houghThreshold, "houghThreshold", 59);
        CONFIG(settings, kernelSize, "kernelSize", 3);
        CONFIG(settings, downSamplingFactor, "downSamplingFactor", 4);
        CONFIG(settings, averageCircleFilterSize, "circleFilterSize", 35);
        CONFIG(settings, circleFilterRadiusDifference, "circleFilterRadiusDifference", 4);
        CONFIG(settings, maskOutForeground, "maskOutForeground", true);
        CONFIG(settings, detectionThreshold, "detectionThreshold", 4000);

        //  This is left here for tuning purposes
        cv::namedWindow("Sliders");
        cv::createTrackbar("detectionThreshold", "Sliders", &detectionThreshold,60000, [](int,void*){});
        cv::createTrackbar("Low thresh", "Sliders", &lowThreshold,255, [](int,void*){});
        cv::createTrackbar("High thresh", "Sliders", &highThreshold,255, [](int,void*){});
        cv::createTrackbar("Hough thresh", "Sliders", &houghThreshold,255, [](int,void*){});
        cv::createTrackbar("Circle size", "Sliders", &averageCircleFilterSize,101, [](int,void*){});
        cv::createTrackbar("Circle thickness", "Sliders", &circleFilterRadiusDifference,10, [](int,void*){});

        makeCircleFilters(circleFilters,circleFilterRadiusDifference,averageCircleFilterSize);

        return true;
    }

    void CircleDetection::process(FrameList &frames)
    {
        for (CameraObject & camera: frames.getCurrent().getCameras()) {
            if(!camera.hasImage("rawImage"))
            {
                LOG("PersonDetection Error", "Image \"rawImage\" not set in current frame!");
                return;
            }
            // Debug stuff (Allows for changing filter size on the fly)
            makeCircleFilters(circleFilters,circleFilterRadiusDifference,averageCircleFilterSize);

            cv::Mat smallImage;
            cv::Mat red, green, blue;
            std::vector<cv::Mat> channels;
            channels.push_back(blue);
            channels.push_back(green);
            channels.push_back(red);
            cv::resize(camera.getImage("rawImage"), smallImage, cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_AREA);
            cv::blur(smallImage, smallImage, cv::Size(kernelSize, kernelSize) );

            cv::split(smallImage , channels);

            cv::Canny( channels[0], channels[0], lowThreshold, highThreshold, kernelSize );
            cv::Canny( channels[1], channels[1], lowThreshold, highThreshold, kernelSize );
            cv::Canny( channels[2], channels[2], lowThreshold, highThreshold, kernelSize );

            cv::Mat canny = cv::max(channels[0], cv::max(channels[1], channels[2]));
            if (camera.hasImage("foregroundMask") && maskOutForeground) {
                cv::Mat foregroundMask;
                cv::resize(camera.getImage("foregroundMask"), foregroundMask,
                           cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_AREA);
                cv::bitwise_and(canny,foregroundMask,canny);
            }

            camera.addImage( "RGB canny", canny );
            cv::Mat printMap;

            // Correllation accumulator variable
            cv::Mat accCircles = cv::Mat::zeros(canny.rows, canny.cols, CV_32FC1);
            for(int i = 0; i < circleFilters.size(); ++i) {
                cv::Mat circleMap;
                cv::filter2D(canny, circleMap, CV_32FC1, circleFilters[i]);

                //accCircles += circleMap; // accumulate
                accCircles = cv::max(accCircles,circleMap);

                // This is all debug stuff (scaling such that correlation is visible in the GUI)
                cv::normalize(circleMap,circleMap,255,0);
                circleMap.convertTo(circleMap ,CV_8UC1);
                circleMap = circleMap*8; // Scale so that circle correlation is clearly visible
                std::string imageName = "Circle map ";
                imageName.append(std::to_string(i));
                camera.addImage( imageName, circleMap );
            }

            qDebug() << cv::norm(accCircles, cv::NORM_INF);
            cv::namedWindow("Accumulated filters",cv::WINDOW_NORMAL);
            cv::imshow("Accumulated filters",accCircles*(1.0/50000));

            cv::Mat thresholded;
            cv::threshold(accCircles,thresholded,detectionThreshold,1,cv::THRESH_BINARY);
            cv::namedWindow("Threshold image");
            cv::imshow("Threshold image",thresholded);

        }
    }
    void CircleDetection::makeCircleFilters(std::vector<cv::Mat> & filters, int circleThickness, int _avgFilterSize)
        {
            int avgFilterSize = _avgFilterSize;
            if (avgFilterSize % 2 != 1) {
                LOG("PersonDetector Warning", "Average circle filter size needs to be an odd number, decreasing size by 1.");
                avgFilterSize--;
            }
            if (avgFilterSize < circleThickness*4) {
                avgFilterSize = circleThickness*4 + 1;
            }

            int deltaRadius = circleThickness;

            std::vector<int> filterSizes;
            filterSizes.push_back(avgFilterSize-deltaRadius);
            filterSizes.push_back(avgFilterSize);
            filterSizes.push_back(avgFilterSize+deltaRadius);
            filters.clear();
            for (unsigned int i = 0; i < filterSizes.size(); ++i) {
                int filterSize = filterSizes[i];
                int radius = (filterSize-1)/2;
                cv::Point center( radius, radius );

                //Make test circles to decide scaling factors
                cv::Mat testNegative = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
                cv::Mat testPositive = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
                cv::circle(testNegative, center, radius, -1,-1);
                cv::circle(testPositive, center, radius-deltaRadius, 1, deltaRadius);
                float negSum=0;
                float posSum=0;
                for (int i = 0; i < filterSize*filterSize; i++) {
                    posSum += testPositive.ptr<float>()[i];
                    negSum += testNegative.ptr<float>()[i];
                }
                assert(negSum < 0 );

                //Make filter with negative center
                float posWeight = std::abs(-posSum - negSum)/posSum;
                cv::Mat filter = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
                cv::circle(filter, center, radius, -1,-1);
                cv::circle(filter, center, radius -deltaRadius , posWeight ,deltaRadius);
                filters.push_back(filter);


                //Make filter with zero in center
                cv::Mat filterZero = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
                cv::circle(filterZero, center, radius -deltaRadius , 1/posSum ,deltaRadius);
                filters.push_back(filter);
            }
        }
    /*
    void CircleDetection::makeCircleFilters(std::vector<cv::Mat> & filters, int numCircles)
    {
        filters.clear();
        for (int i = 0; i < circleFilterSizes.size(); ++i) {
            int filterSize = circleFilterSizes[i];
            int radius = (filterSize-1)/2;
            cv::Point center( radius, radius );

            //Test image used for calculating the size of the negative portion of the filter
            cv::Mat testNegative = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
            cv::circle(testNegative, center, radius, -1,-1);
            float negSum=0;
            cv::MatConstIterator_<float> itn = testNegative.begin<float>(), itn_end = testNegative.end<float>();
            for(; itn != itn_end; ++itn) {
                negSum += *itn;
            }

            //Test image used for calculating the size of the positive portion of the filter
            cv::Mat testPositive = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
            cv::circle(testPositive, center, radius-circleFilterRadiusDifference,
                                                    1 ,circleFilterRadiusDifference);
            float posSum=0;
            cv::MatConstIterator_<float> itp = testPositive.begin<float>(), itp_end = testPositive.end<float>();
            for(; itp != itp_end; ++itp) {
                posSum += *itp;
            }
            float posWeight = std::abs(-posSum - negSum)/posSum;

            cv::Mat filter = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
            cv::circle(filter, center, radius, -1,-1);
            cv::circle(filter, center, radius -circleFilterRadiusDifference,
                                        posWeight ,circleFilterRadiusDifference);
            filters.push_back(filter);
        }
    }*/

    void CircleDetection::gradientCircleVoting(cv::Mat &grayImage)
        {
            cv::Mat resultImage = cv::Mat::zeros(grayImage.rows,grayImage.cols, CV_64FC1);
            int searchSize = 37;
            int searchWidth = std::ceil((float)searchSize/2);

            // Generate grad_x and grad_y
            cv::Mat dx, dy;
            Scharr( grayImage, dx, CV_64F, 1, 0);
            Scharr( grayImage, dy, CV_64F, 0, 1);

            //Not exactly an optimal loop.. =) To do it fast, first vreate a matrix of gradient points and one with displacement points
            double maxScore = 0;
            for (int row = 0; row < dx.rows; ++row) {
                double * resultRow = resultImage.ptr<double>(row);
                for (int col = 0; col < dx.cols; ++col) {
                    double score = 0;
                    int minSearchRow = std::max(0, row - searchWidth);
                    int maxSearchRow = std::min(dx.rows, row + searchWidth);
                    int minSearchCol = std::max(0, col - searchWidth);
                    int maxSearchCol = std::min(dx.cols, col + searchWidth);
                    for (int sRow = minSearchRow; sRow < maxSearchRow; ++sRow) {
                        double * dxRow = dx.ptr<double>(sRow);
                        double * dyRow = dy.ptr<double>(sRow);
                        for (int sCol = minSearchCol; sCol < maxSearchCol; ++sCol) {
                            if (!(sCol == col && sRow == row)) {
                                cv::Point2d disp(sCol - col, sRow - row);
                                disp = disp*(1/cv::norm(disp));
                                cv::Point2d gradient(dxRow[sCol],dyRow[sCol]);
                                score += disp.ddot(gradient);
                            }
                        }
                    }
                    maxScore = std::max(score,maxScore);
                    resultRow[col] = score;
                }
            }
            resultImage = resultImage/maxScore;

            cv::namedWindow("Circle voting");
            cv::imshow("Circle voting",resultImage);
        }

}
