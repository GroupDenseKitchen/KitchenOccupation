#ifndef DEBUGVIEW_HPP
#define DEBUGVIEW_HPP

#include <QDialog>
#include <QString>

namespace Ui {
class DebugView;
}

class DebugView : public QDialog
{
    Q_OBJECT

public:
    explicit DebugView(QWidget *parent = 0);
    ~DebugView();

    void setName(QString);
private:
    Ui::DebugView *ui;
    QString name;
};

#endif // DEBUGVIEW_HPP
