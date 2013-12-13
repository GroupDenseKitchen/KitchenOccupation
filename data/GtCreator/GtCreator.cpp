#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <vector>
#include <iostream>

void createConfigFile(std::string cfgFile)
{
    // Open file
    cv::FileStorage textFile(cfgFile,cv::FileStorage::WRITE);
    // Add data to file
    textFile << "videoFilePath" << "/Users/erikfall/Desktop/EvalClip.mp4";
    textFile << "GTDataFilePath" << "/Users/erikfall/Desktop/Eval.yml";
    // Saves file
    textFile.release();
}

int main()
{
    // Creates file with video file path
    createConfigFile("myFile.yml");

    // Reads file path from file
    cv::FileStorage file("myFile.yml",cv::FileStorage::READ);
    std::string videoFilePath, GTDataFilePath;
    file["videoFilePath"] >> videoFilePath;
    file["GTDataFilePath"] >> GTDataFilePath;
    file.release();

    // Creates a window
    cv::namedWindow("VideoWindow");

    // Reads video from file path
    cv::VideoCapture videoFile(videoFilePath);
    cv::Mat frame;
    char c;
    int entryFrame = 0;
    int exitFrame = 0;
    std::vector<int> entries;
    std::vector<int> exits;


    if (videoFile.isOpened()) {
        // While frames left
        while (videoFile.read(frame)) {
            double currentFrame = videoFile.get(CV_CAP_PROP_POS_FRAMES);


            // Print entries and exits in frame
            std::string text = "", text1 = "", text2 = "";
            std::stringstream s, s1, s2;
            s << currentFrame;
            s1 << entryFrame;
            s2 << exitFrame;
            text = "Frame number: " + s.str();
            text1 = "Entries: " + s1.str();
            text2 = "Exits: " + s2.str();
            cv::Point2d pos(10,25);
            cv::Point2d pos1(10,45);
            cv::Point2d pos2(10,65);
            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 1;
            int thickness = 1;
            cv::putText(frame, text, pos, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            cv::putText(frame, text1, pos1, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            cv::putText(frame, text2, pos2, fontFace, fontScale, cv::Scalar(255,0,0), thickness, 8);
            cv::imshow("MahVidyaWindow",frame);





            c = cv::waitKey(0); // Wait for any key to be pressed

            // To exit the program, using esc
            if (c == 'q'){
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                break;
            }
            // To back one step, using z
            else if (c == 'z'){
                entryFrame = entries.back();
                exitFrame = exits.back();
                entries.pop_back();
                exits.pop_back();
                videoFile.set(CV_CAP_PROP_POS_FRAMES, currentFrame-1);
            }
            else if (c == '<'){ //Back faster
                entryFrame = entries.back();
                exitFrame = exits.back();
                entries.pop_back();
                exits.pop_back();

                entryFrame = entries.back();
                exitFrame = exits.back();
                entries.pop_back();
                exits.pop_back();

                entryFrame = entries.back();
                exitFrame = exits.back();
                entries.pop_back();
                exits.pop_back();

                entryFrame = entries.back();
                exitFrame = exits.back();
                entries.pop_back();
                exits.pop_back();

                videoFile.set(CV_CAP_PROP_POS_FRAMES, currentFrame-4);
            }
            // To step forward, using x
            else if (c == 'x'){
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entryFrame = 0;
                exitFrame = 0;
                videoFile.set(CV_CAP_PROP_POS_FRAMES, currentFrame+1);
            }
            else if (c == 'c'){  // forward faster!
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entryFrame = 0;
                exitFrame = 0;
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                videoFile.set(CV_CAP_PROP_POS_FRAMES, currentFrame+7);
            }

            // Someone exits, using -
            else if(c == '-'){
                exitFrame = exitFrame + 1;
                videoFile.set(CV_CAP_PROP_POS_FRAMES, currentFrame);
            }
            // Someone enters, using +
            else if (c == '+'){
                entryFrame = entryFrame + 1;
                videoFile.set(CV_CAP_PROP_POS_FRAMES, currentFrame);
            }
            else{
                entries.push_back(entryFrame);
                exits.push_back(exitFrame);
                entryFrame = 0;
                exitFrame = 0;
                videoFile.set(CV_CAP_PROP_POS_FRAMES, currentFrame-1);
            }



        }

        // Save vectors to file
        cv::FileStorage textFile(GTDataFilePath, cv::FileStorage::WRITE);
        textFile << "Entries" << entries;
        textFile << "Exits" << exits;
        textFile.release();

    } else {
        std::cerr << "Failed to open video file at :" << videoFilePath << std::endl;
        cv::waitKey(0);
    }

    return 0;
}
