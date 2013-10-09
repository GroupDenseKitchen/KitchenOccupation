#ifndef CVIMAGEWIDGET_HPP
#define CVIMAGEWIDGET_HPP

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>

class CvImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CvImageWidget(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
signals:

public slots:
    void showImage(const cv::Mat image);

protected:

    void paintEvent(QPaintEvent *);

QImage qtImage;
};

#endif // CVIMAGEWIDGET_HPP
