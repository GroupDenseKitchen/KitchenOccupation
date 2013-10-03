#ifndef CAMRECWINDOW_H
#define CAMRECWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <opencv2/opencv.hpp>

namespace Ui {
class CamRecWindow;
}

class CamRecWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CamRecWindow(QWidget *parent = 0);
    ~CamRecWindow();

    cv::VideoCapture *camera;
    cv::VideoWriter videoWriter;
    std::string writePath;
    int fps;
    bool isRecording;

    cv::Mat matImage;
    QImage qImage;

    QTimer* timer;
    int timerDelay;
    QElapsedTimer* elapsedTimer;
    qint64 uppdateTime;

public slots:
    void cameraUpdate();

private slots:
    void on_recordButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::CamRecWindow *ui;
};

#endif // CAMRECWINDOW_H
