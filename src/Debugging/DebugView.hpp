#ifndef DEBUGVIEW_HPP
#define DEBUGVIEW_HPP

#include <QDialog>
#include <QString>
#include "CvImageWidget.hpp"

namespace Ui {
class DebugView;
}

class DebugView : public QDialog
{
    Q_OBJECT

public:
    explicit DebugView(QWidget *parent = 0);
    ~DebugView();

    void init(const QString);
    void showImage(const cv::Mat& image);

private:
    Ui::DebugView *ui;
    QString name;
    CvImageWidget * debugImageWidget;
};

#endif // DEBUGVIEW_HPP
