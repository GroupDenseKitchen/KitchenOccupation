#include "CamRecWindow.h"
#include "ui_CamRecWindow.h"

CamRecWindow::CamRecWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CamRecWindow)
{
    ui->setupUi(this);

    // Try to connect to specified network camera

    // http://USERNAME:PASSWORD@IP-ADRESS/video.cgi?INTERNAL_SETTINGS&.mjpg
    // &.mjpg at the end to help OpenCV find the right codec
    camera = new cv::VideoCapture("http://root:pass@169.254.192.68/axis-cgi/mjpg/video.cgi?resolution=640x480&.mjpg");
    if(!camera->isOpened()){
        // If network camera fails, open default camera
        camera = new cv::VideoCapture(0);
        if(camera->isOpened()){
            ui->textEdit->setText("Default Camera Opened");
        } else {
            ui->textEdit->setText("No Camera Found");
        }
    } else {
        ui->textEdit->setText("Network Camera Opened");
    }

    writePath = "videoStream.avi";
    fps = 10;
    isRecording = false;


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(cameraUpdate()));
    timerDelay = 1000/fps;
    timer->start(timerDelay);
    elapsedTimer = new QElapsedTimer;
}

CamRecWindow::~CamRecWindow()
{
    delete ui;
    camera->release();
    videoWriter.release();
}

void CamRecWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
    case Qt::Key_Escape:
        close();
        break;
    default:
        break;
    }
}

void CamRecWindow::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void CamRecWindow::cameraUpdate(){
    elapsedTimer->start();
    // Fetch image
    camera->read(matImage);

    // Get out if no image exists
    if(matImage.empty()) return;

    cv::imshow("", matImage);

    /*
    if(isRecording){
        videoWriter.write(matImage);
    } else {
        // Convert image to QImage to make displayable in the image label
        cv::cvtColor(matImage, matImage, CV_BGR2RGB);
        qImage = QImage((uchar*)matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGB888);

        ui->imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }
    uppdateTime = elapsedTimer->elapsed();
    ui->textEdit->append("UppdateTime = " + QString::number(uppdateTime) + " ms");
    int adjustedTimer = timerDelay - uppdateTime;
    if (adjustedTimer < 0){
        timer->start(0);
    } else {
        timer->start(timerDelay - uppdateTime);
    }
    */
}

void CamRecWindow::on_recordButton_clicked()
{
    if(videoWriter.isOpened()){
        isRecording = true;
        ui->textEdit->append("Recording started");
    } else {
        if(camera->isOpened()){
            cv::Size size = matImage.size();
            bool color = true;
            videoWriter.open(writePath, CV_FOURCC('M','J','P','G'), (double)fps, size, color);
            if(videoWriter.isOpened()){
                isRecording = true;
                ui->textEdit->append("Video Writer just opened");
                    ui->textEdit->append("Recording started");
            } else {
                ui->textEdit->append("The Video Stream could not be opened");
            }
        } else {
            ui->textEdit->append("No camera is opened");
        }
    }
}

void CamRecWindow::on_stopButton_clicked()
{
    isRecording = false;
    ui->textEdit->append("Recording stopped");
}
