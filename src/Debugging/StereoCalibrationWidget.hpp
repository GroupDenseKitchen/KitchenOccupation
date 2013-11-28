#ifndef STEREOCALIBRATIONWIDGET_HPP
#define STEREOCALIBRATIONWIDGET_HPP

#include <QWidget>
#include <QCloseEvent>

#include "../Utilities/Frame.hpp"

namespace Ui {
class StereoCalibrationWidget;
}

class StereoCalibrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StereoCalibrationWidget(QWidget *parent = 0);
    ~StereoCalibrationWidget();

    void showImage();
    void storeImage(const cv::Mat &image);

    cv::Mat matImage;

public slots:
    void updateWindow(Frame currentFrame);


private:
    Ui::StereoCalibrationWidget *ui;
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);
};

#endif // STEREOCALIBRATIONWIDGET_HPP
