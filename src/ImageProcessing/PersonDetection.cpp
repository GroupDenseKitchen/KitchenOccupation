#include "PersonDetection.hpp"

#include <QDebug>

namespace image_processing
{

    PersonDetection::PersonDetection()
    {
    }

    bool PersonDetection::initialize(configuration::ConfigurationManager &settings)
    {
        CONFIG(settings, lowThreshold, "lowThreshold", 100);
        CONFIG(settings, highThreshold, "highThreshold", 200);
        CONFIG(settings, kernelSize, "kernelSize", 3);
        CONFIG(settings, downSamplingFactor, "downSamplingFactor", 4);
        CONFIG(settings, circleFilterRadiusDifference, "circleFilterRadiusDifference", 4);

        /*  This is left here for tuning purposes
        cv::namedWindow("Sliders");
        cv::createTrackbar("Low thresh", "Sliders", &lowThreshold,100, [](int,void*){});
        cv::createTrackbar("High thresh", "Sliders", &highThreshold,200, [](int,void*){});
        */
        makeCircleFilters(circleFilters,1);

        return true;
    }

    void PersonDetection::process(FrameList &frames)
    {
        for (CameraObject & camera: frames.getCurrent().getCameras()) {
            if(!camera.hasImage("rawImage"))
            {
                LOG("PersonDetection Error", "Image \"rawImage\" not set in current frame!");
                return;
            }

            cv::Mat gray;
            cvtColor( camera.getImage("rawImage"), gray, CV_RGB2GRAY);

            /*//Create foreground mask from object rectangles
            std::vector<Object> objects = camera.getObjects();
            cv::Mat mask = cv::Mat::zeros( gray.rows, gray.cols, CV_8U );
            for (Object & object: objects) {
                mask( object.boundingBox ) = 1;
            }

            cv::Mat croppedGray;
            gray.copyTo( croppedGray,  mask );



            // Reduce noise with a kernel 3x3
            cv::blur( croppedGray, croppedGray, cv::Size(kernelSize, kernelSize) );*/

             cv::Mat raw = camera.getImage("rawImage");

            // Canny detector
            cv::Mat canny, cannyGray;
            cv::Mat red, green, blue;
            std::vector<cv::Mat> channels = {blue, green, red};
            cv::split( raw , channels);

            // Downsampling stuff
            cv::Mat smallRed, smallBlue, smallGreen, smallGray;
            cv::resize(channels[2],smallRed,cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_AREA);
            cv::resize(channels[1],smallGreen,cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_AREA);
            cv::resize(channels[0],smallBlue,cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_AREA);


            cv::blur( smallRed,smallRed, cv::Size(kernelSize, kernelSize) );
            cv::blur( smallGreen,smallGreen, cv::Size(kernelSize, kernelSize) );
            cv::blur( smallBlue,smallBlue, cv::Size(kernelSize, kernelSize) );
            cv::Mat cannyRed, cannyBlue, cannyGreen;


            cv::Canny( smallRed, cannyRed, lowThreshold, highThreshold, kernelSize );
            cv::Canny( smallBlue, cannyBlue, lowThreshold, highThreshold, kernelSize );
            cv::Canny( smallGreen, cannyGreen, lowThreshold, highThreshold, kernelSize );
            canny = cv::max (cannyRed, cv::max(cannyGreen, cannyBlue) );

            //cv::blur( gray, gray, cv::Size(kernelSize, kernelSize) );
            cv::resize(gray,smallGray,cv::Size(0,0), 1/downSamplingFactor,1/downSamplingFactor, CV_INTER_NN);
            cv::Canny( smallGray, cannyGray, lowThreshold, highThreshold, kernelSize );

            cv::Mat printMap;
            for(int i = 0; i < 1; i++) {//circleFilters.size(); ++i) {
                cv::Mat circleMap;
                cv::filter2D(canny, circleMap, CV_32FC1,circleFilters[i]);
                cv::normalize(circleMap,circleMap,255,0);

                circleMap.convertTo(printMap ,CV_8UC1);
                printMap = printMap*4;
                std::string imageName = "Circle map ";
                imageName.append(std::to_string(i));
                camera.addImage( imageName, printMap*2 );
            }

            camera.addImage( "RGB canny", canny );
            camera.addImage( "Gray Canny" ,cannyGray );
            cv::namedWindow("Circle correlation");
            cv::namedWindow("Thresholded circle correlation");
            cv::Mat showMap;
            cv::resize(printMap,showMap,cv::Size(0,0),downSamplingFactor,downSamplingFactor,CV_INTER_NN);
            cv::imshow("Circle correlation", showMap*2);
            cv::threshold(showMap*2, showMap, 59, 255, CV_THRESH_BINARY);
            cv::imshow("Thresholded Circle correlation", showMap*2);
            cv::waitKey(1);

        }
    }

    void PersonDetection::makeCircleFilters(std::vector<cv::Mat> & filters, int numCircles)
    {
        filters.clear();
        for (int i = 0; i < circleFilterSizes.size(); ++i) {
            int filterSize = circleFilterSizes[i];
            int radius = (filterSize-1)/2;
            cv::Point center( radius, radius );

            cv::Mat testNegative = cv::Mat::zeros(filterSize, filterSize, CV_32FC1);
            cv::circle(testNegative, center, radius, -1,-1);
            float negSum=0;
            cv::MatConstIterator_<float> itn = testNegative.begin<float>(), itn_end = testNegative.end<float>();
            for(; itn != itn_end; ++itn) {
                negSum += *itn;
            }

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
    }

}
