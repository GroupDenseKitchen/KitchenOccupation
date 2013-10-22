#ifndef DEBUGVIEW_HPP
#define DEBUGVIEW_HPP

#include <QDialog>
#include <string>
#include "CvImageWidget.hpp"
#include "../Utilities/Frame.hpp"

namespace Ui {
class DebugView;
}

class DebugView : public QDialog
{
    Q_OBJECT

public:
    explicit DebugView(QWidget *parent = 0);
    ~DebugView();

    void init(const std::string processStepName, int camNumber);
    void showImage(const cv::Mat& image);

public slots:
    void updateView( Frame );

private:
    Ui::DebugView *ui;
    std::string processStep;
    int cameraNumber;
    CvImageWidget * debugImageWidget;
};

#endif // DEBUGVIEW_HPP
