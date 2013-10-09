#include "CvImageWidget.hpp"
#include <QDebug>
#include "../Utilities/utilities.hpp"
#include <opencv2/core/core.hpp>

CvImageWidget::CvImageWidget(QWidget *parent) :
    QWidget(parent)
{
}

QSize CvImageWidget:: sizeHint() const
{
    return qtImage.size();
}


QSize CvImageWidget:: minimumSizeHint() const
{
    return qtImage.size();
}

void CvImageWidget:: showImage(const cv::Mat image)
{
    cv::Mat temporaryCvImage;
    switch (image.type()) {
    case CV_8UC1:
        cv::cvtColor(image, temporaryCvImage, CV_GRAY2RGB );
        break;
    case CV_8UC3:
        cv::cvtColor(image, temporaryCvImage, CV_BGR2RGB );
        break;
    default :
        qDebug() << "Unexpected CV image format";
        break;
    }

    assert(temporaryCvImage.isContinuous());

    qtImage = QImage(temporaryCvImage.data,
                     temporaryCvImage.cols,
                     temporaryCvImage.rows,
                     temporaryCvImage.cols*3,
                     QImage::Format_RGB888);

    this->setFixedSize(image.cols,image.rows);

    repaint();
}

void CvImageWidget:: paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), qtImage);
    painter.end();
}
