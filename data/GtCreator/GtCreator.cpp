#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>

void createConfigFile(std::string cfgFile)
{
    cv::FileStorage textFile(cfgFile,cv::FileStorage::WRITE);
    textFile << "myString" << "Hello OpenCV text file handle";
    textFile << "myVideoFilePath" << "../seq3/seq3.mp4";
    textFile << "myImageFilePath" << "image.png";
    std::vector<int> integers;
    integers.push_back(0);
    integers.push_back(1);
    integers.push_back(2);
    integers.push_back(3);
    integers.push_back(1337);

    textFile << "myIntVector" << integers;

    textFile.release();
}

int main()
{
    createConfigFile("myFile.yml");

    cv::FileStorage file("myFile.yml",cv::FileStorage::READ);
    std::string videoFilePath, imageFilePath;

    file["myImageFilePath"] >> imageFilePath;
    file["myVideoFilePath"] >> videoFilePath;
    file.release();

    cv::namedWindow("MahImageWindow");
    cv::namedWindow("MahVidyaWindow");

    cv::Mat image = cv::imread(imageFilePath);
    if (!image.empty()) {

        cv::putText(image,"Hello OpenCV", cv::Point(20,30), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(50,50,50));
        cv::imshow("MahImageWindow", image);
        cv::waitKey(0);
    } else {
        std::cerr << "Failed to open image at :" << imageFilePath << std::endl;
    }

    cv::VideoCapture videoFile(videoFilePath);
    cv::Mat frame;
    char c;
    if (videoFile.isOpened()) {
        // While we have frames left
        while (videoFile.read(frame)) {
            cv::imshow("MahVidyaWindow",frame);
            c = cv::waitKey(0); // Wait for any key to be pressed
            if (c == 27)
                break;
        }
    } else {
        std::cerr << "Failed to open video file at :" << videoFilePath << std::endl;
        cv::waitKey(0);
    }

    return 0;
}
