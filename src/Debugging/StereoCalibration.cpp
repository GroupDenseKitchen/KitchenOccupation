#include "StereoCalibration.hpp"
#include "ui_StereoCalibration.h"

StereoCalibration::StereoCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StereoCalibration)
{
    ui->setupUi(this);
}

StereoCalibration::~StereoCalibration()
{
    delete ui;
}
