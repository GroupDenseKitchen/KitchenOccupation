#include "PersonDetection.hpp"

#include <QDebug>

namespace image_processing
{

    PersonDetection::PersonDetection()
    {
    }

    bool PersonDetection::initialize(configuration::ConfigurationManager &settings)
    {
        //Defaults
        CONFIG(settings, detectorType, "detectorType", "ORB");
        CONFIG(settings, descriptorType, "descriptorType", "ORB");
        CONFIG(settings, matcherType, "matcherType", "BruteForce-Hamming");

        detector = cv::FeatureDetector::create( detectorType );
        descriptorExtractor = cv::DescriptorExtractor::create( descriptorType );
        descriptorMatcher = cv::DescriptorMatcher::create( matcherType );

        currentKeyPoints = new std::vector<cv::KeyPoint>();
        previousKeyPoints = new std::vector<cv::KeyPoint>();

        cv::namedWindow("Circles", CV_WINDOW_AUTOSIZE);

        cv::createTrackbar("Min dist", "Circles", &minDist,400, [](int,void*){});
        cv::createTrackbar("Low thresh", "Circles", &lowThreshold,400, [](int,void*){});
        cv::createTrackbar("High thresh", "Circles", &highThreshold,400, [](int,void*){});
        cv::createTrackbar("Kernel size", "Circles", &kernelSize,12, [](int,void*){});

        if( detector.empty() || descriptorExtractor.empty() || descriptorMatcher.empty()  )
            {
                LOG("PersonDetection Error", "Could not create descriptor, extractor or matcher");
                return false;
            }
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
            cvtColor( camera.getImage("rawImage").clone(), gray, CV_RGB2GRAY);
            cv::Mat raw = camera.getImage("rawImage");

            // Reduce noise with a kernel 3x3
            cv::blur( gray, gray, cv::Size(kernelSize, kernelSize) );

            // Canny detector
            cv::Mat canny;
            cv::Canny( gray, canny, lowThreshold, highThreshold, kernelSize );

            std::vector<cv::Vec3f> circles;
            cv::HoughCircles( gray, circles, CV_HOUGH_GRADIENT, 1, minDist,
                              highThreshold, lowThreshold, 0, 0 );
            LOG("Number of detected circles", circles.size() );
            // Draw the circles detected
              for( size_t i = 0; i < circles.size(); i++ )
              {
                  cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                  int radius = cvRound(circles[i][2]);
                  // circle center
                  circle( raw, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
                  // circle outline
                  circle( raw, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
               }

              cv::imshow("Circles",raw);
            camera.addImage( "Person Detection Experiments", canny );
        }
    }

    void PersonDetection::calculatePointMatches()
    {
        /*if(!camera.hasImage("rawImage"))
        {
            LOG("PersonDetection Error", "Image \"rawImage\" not set in current frame!");
            return;
        }
        previousImage = currentImage.clone();
        cvtColor( camera.getImage("rawImage").clone(), currentImage, CV_RGB2GRAY);
        if (previousImage.empty() || currentImage.empty() ) {
            return;
        }

        previousKeyPoints = currentKeyPoints;
        currentKeyPoints = new std::vector<cv::KeyPoint>();
        detector->detect( currentImage, *currentKeyPoints );

        previousDescriptors = currentDescriptors;
        descriptorExtractor->compute( currentImage, *currentKeyPoints, currentDescriptors );

        if ( previousKeyPoints->empty() ) {
            return;
        }
        std::vector<cv::DMatch> matches;
        descriptorMatcher->match( previousDescriptors, currentDescriptors, matches );

        cv::Mat debugImage;
        qDebug() << "Size of previousKeypoints vector" << previousKeyPoints->size();
        qDebug() << "Size of currentKeypoints vector" << currentKeyPoints->size();
        cv::drawMatches( previousImage, *previousKeyPoints,
                         currentImage, *currentKeyPoints,
                         matches, debugImage,CV_RGB(0, 255, 0), CV_RGB(0, 0, 255) );
        camera.addImage( "Person Detection Experiments", debugImage );
        */
    }

    void PersonDetection::calculateGradientMagnitude(cv::Mat &grayImg, cv::Mat &gradientMag)
    {
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;

        // Generate grad_x and grad_y
        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;

        // Gradient X
        //Scharr( rawGray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
        Sobel( grayImg, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
        convertScaleAbs( grad_x, abs_grad_x );

        // Gradient Y
        //Scharr( rawGray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
        Sobel( grayImg, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
        convertScaleAbs( grad_y, abs_grad_y );

        // Total Gradient (approximate)
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradientMag );

    }
}
