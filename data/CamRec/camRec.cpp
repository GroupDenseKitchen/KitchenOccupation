#include <QDebug>
#include <QTime>
#include <QElapsedTimer>

#include <opencv2/opencv.hpp>

int main()
{
    std::string fileName = "videoStream";

    QDateTime current = QDateTime::currentDateTime();
    QTime time = QTime::currentTime();
    std::string timeStamp = time.toString(Qt::TextDate).toStdString();

    std::cerr << timeStamp << std::endl;


    std::string fileExtension = ".avi";
    std::string writePath = fileName + fileExtension;

    std::cerr << writePath << std::endl;

    double fps = 30;
    cv::Size size(640,480);

    cv::VideoCapture* camera = new cv::VideoCapture("http://root:pass@169.254.234.112/axis-cgi/mjpg/video.cgi?resolution=640x480&.mjpg");
    //cv::VideoCapture* camera = new cv::VideoCapture(CV_CAP_OPENNI);

    if (!camera->isOpened()){
        qDebug() << "Det gick inte att öppna kameran";
        exit(-1);
    }
    //cv::VideoWriter* videoWriter = new cv::VideoWriter(writePath, CV_FOURCC('D','I','V','X'), fps, size) ;

    cv::Mat image;
    char key;

    bool recording = true;
    int frames = 0;

    QElapsedTimer timer;
    timer.start();
    qint64 workTime;

    while(recording){

        timer.restart();

        camera->read(image);
        //videoWriter->write(image);
        //cv::imshow("", image);

        key = cv::waitKey(5);
        if(key == 'q'){
            qDebug() << "escaping";
            recording = false;
        }

        frames++;
        qDebug() << frames;

        workTime = timer.elapsed();

        qDebug() << "WorkTime: " << workTime;

        int delay = 33 - (int)workTime;

        if( delay < 1 ){
            delay = 1;
        }
        cv::waitKey(delay);
        std::cerr << timer.elapsed() << std::endl;
    }

    camera->release();
    //videoWriter->release();

    return 0;
}
