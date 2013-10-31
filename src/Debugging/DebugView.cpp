#include "DebugView.hpp"
#include "ui_DebugView.h"
#include "MainDebugWindow.hpp"
#include <QDebug>
#include <QString>

DebugView::DebugView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugView)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    connect( parentWidget(), SIGNAL(updateDebugViews(Frame)),
             this, SLOT(updateView(Frame)) );
}

DebugView::~DebugView()
{
    disconnect( parentWidget(), SIGNAL(updateDebugViews(Frame)),
                this, SLOT(updateView(Frame)) );
    delete ui;
}

void DebugView:: init(const std::string processStepName, int camNumber)
{
    processStep = processStepName;
    cameraNumber = camNumber;
    QString processStepNameQ = processStep.c_str();
    setWindowTitle("Camera: " + QString::number(cameraNumber) +
                   " Process Step: " + processStepNameQ);

    debugImageWidget = new CvImageWidget(this);
    ui->verticalLayout->addWidget(debugImageWidget);
}

void DebugView::showImage(const cv::Mat& image)
{
    debugImageWidget->showImage(image);
}

void DebugView::updateView( Frame currentFrame)
{
    if ( cameraNumber < currentFrame.getCameras().size() ) {
        CameraObject cam = currentFrame.getCameras()[cameraNumber];
        if (cam.hasImage(processStep)) {
            debugImageWidget->showImage(cam.getImage(processStep));
        } else {
            //TODO: use LOG here instead?
            qDebug() << "Process step doesn't exist in DebugView::updateView";
        }
    } else {
        qDebug() << "Camera doesn't exist in DebugView::updateView";
    }
}
