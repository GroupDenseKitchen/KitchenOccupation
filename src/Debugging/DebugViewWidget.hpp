#ifndef DEBUGVIEWWIDGET_HPP
#define DEBUGVIEWWIDGET_HPP

#include "CvImageWidget.hpp"
#include "../Utilities/Frame.hpp"
#include <QWidget>
#include <opencv2/core/core.hpp>

namespace Ui {
class DebugViewWidget;
}

class DebugViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugViewWidget(QWidget *parent = 0);
    ~DebugViewWidget();

    void init(const std::string processStepName, int camNumber);
    void showImage(const cv::Mat& image);
    std::string getIdentifier();

public slots:
    void updateView( Frame );

signals:
    void aboutToClose(std::string);

private:
    Ui::DebugViewWidget *ui;
    std::string processStep;
    int cameraNumber;

    cv::Mat matImage;
    QImage qImage;

    void closeEvent(QCloseEvent *);
};

#endif // DEBUGVIEWWIDGET_HPP
