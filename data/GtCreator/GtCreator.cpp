#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>

void createConfigFile(std::string cfgFile)
{
    // Open file
    cv::FileStorage textFile(cfgFile,cv::FileStorage::WRITE);
    // Add data to file
    textFile << "myString" << "Hello OpenCV text file handle";
    textFile << "myVideoFilePath" << "../seq3/seq3.mp4";
    textFile << "myImageFilePath" << "image.png";
    //std::vector<int> integers;
    //integers.push_back(0);
    //integers.push_back(1);
    //integers.push_back(2);
    //integers.push_back(3);
    //integers.push_back(1337);

    //textFile << "myIntVector" << integers;

    // Måste göras för att sparas
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
    int entryFrame = 0;
    int exitFrame = 0;
    std::vector<int> entries;
    std::vector<int> exits;
    entries.reserve(videoFile.get(CV_CAP_PROP_FRAME_COUNT));
    exits.reserve(videoFile.get(CV_CAP_PROP_FRAME_COUNT));

    if (videoFile.isOpened()) {
        // While we have frames left
        while (videoFile.read(frame)) {
            cv::imshow("MahVidyaWindow",frame);
            int actualFrame = videoFile.get(CV_CAP_PROP_POS_FRAMES);

            c = cv::waitKey(0); // Wait for any key to be pressed

            // To exit the program, using esc
            if (c == 27){
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);

                std::cout << "Entry vector: ";
                for(std::vector<int>::const_iterator i=entries.begin(); i!=entries.end(); ++i)
                    std::cout << (*i) << " ";
                std::cout << "\n" << "Exit vector: ";
                for(std::vector<int>::const_iterator i=exits.begin(); i!=exits.end(); ++i)
                    std::cout << (*i) << " ";
                std::cout << "\n";
                break;
            }
            // To back one step, using z
            else if (c == 122){
                entries.pop_back();
                exits.pop_back();
                entryFrame = 0;
                exitFrame = 0;
                actualFrame = videoFile.set(CV_CAP_PROP_POS_FRAMES, actualFrame-2);
            }
            // To step forward, using x
            else if (c == 120){
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entryFrame = 0;
                exitFrame = 0;
            }
            // Someone exits, using -
            else if(c == 45){
                exitFrame = exitFrame + 1;
                actualFrame = videoFile.set(CV_CAP_PROP_POS_FRAMES, actualFrame-1);
            }
            // Someone enters, using +
            else if (c == 43){
                entryFrame = entryFrame + 1;
                actualFrame = videoFile.set(CV_CAP_PROP_POS_FRAMES, actualFrame-1);
            }
            // To ignore all other keys
            else
                actualFrame = videoFile.set(CV_CAP_PROP_POS_FRAMES, actualFrame-1);
        }
        // Save vectors to file
        cv::FileStorage textFile("groundTruth.yml",cv::FileStorage::WRITE);
        textFile << "Entries" << entries;
        textFile << "Exits" << exits;
        textFile.release();

    } else {
        std::cerr << "Failed to open video file at :" << videoFilePath << std::endl;
        cv::waitKey(0);
    }

    return 0;
}
