#include "DebugView.hpp"
#include "ui_DebugView.h"

DebugView::DebugView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugView)
{
    ui->setupUi(this);
}

DebugView::~DebugView()
{
    delete ui;
}

void DebugView:: init(const QString newName)
{
    name = newName;
    ui->label->setText(newName);

    debugImageWidget = new CvImageWidget(this);
    ui->verticalLayout->addWidget(debugImageWidget);
}

void DebugView::showImage(const cv::Mat& image)
{
    debugImageWidget->showImage(image);
}
