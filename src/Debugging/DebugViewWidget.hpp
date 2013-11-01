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
    void showImage();
    std::string getIdentifier();

public slots:
    void updateView( Frame );

signals:
    void aboutToClose(std::string);

private:
    Ui::DebugViewWidget *ui;
    std::string TAG;
    std::string processStep;
    uint cameraNumber;

    cv::Mat matImage, resizedImage;
    QImage qImage;

    void storeImage(const cv::Mat& image);
    void adpatToWidgetSize();
    void adaptImageToWidget();

    int windowWidth;
    int windowHeight;
    float windowRatio;

    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // DEBUGVIEWWIDGET_HPP
