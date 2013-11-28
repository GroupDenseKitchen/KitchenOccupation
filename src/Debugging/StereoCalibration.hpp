#ifndef STEREOCALIBRATION_HPP
#define STEREOCALIBRATION_HPP

#include <QWidget>

namespace Ui {
class StereoCalibration;
}

class StereoCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit StereoCalibration(QWidget *parent = 0);
    ~StereoCalibration();

private:
    Ui::StereoCalibration *ui;
};

#endif // STEREOCALIBRATION_HPP
