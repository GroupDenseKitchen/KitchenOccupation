#ifndef CAMRECORDERWINDOW_H
#define CAMRECORDERWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaRecorder>
#include <QtMultimediaWidgets/QVideoWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class CamRecorderWindow;
}

class CamRecorderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CamRecorderWindow(QWidget *parent = 0);
    ~CamRecorderWindow();

    cv::VideoCapture *cam0, *cam1, *cam2;
    int camNumber;
    cv::VideoWriter videoWriter;
    bool record;

    cv::Mat matOriginal, matProcessed;
    QImage qImgOriginal;

    QTimer* timer;
    QElapsedTimer fpsMeter;
    qint64 nanoSec;
    QKeyEvent* keyEvent;

    int fps;
    bool color;
    bool flip;
    QString fileName;

    void keyPressEvent(QKeyEvent *);

public slots:
    void camUpdate();

private slots:
    void on_fpsSlider_sliderMoved(int position);

    void on_colorCheckBox_clicked(bool checked);

    void on_recordBtn_clicked();

    void on_stopBtn_clicked();

    void on_flipCheckBox_clicked(bool checked);

    void on_camComboBox_activated(int index);

private:
    Ui::CamRecorderWindow *ui;
};

#endif // CAMRECORDERWINDOW_H
