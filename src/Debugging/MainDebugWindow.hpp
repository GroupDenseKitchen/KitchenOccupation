#ifndef MAINDEBUGWINDOW_HPP
#define MAINDEBUGWINDOW_HPP

#include <QMainWindow>
#include <map>

#include "../DenseKitchen.hpp"
#include "DebugView.hpp"

namespace Ui {
class MainDebugWindow;
}

class MainDebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainDebugWindow(QWidget *parent = 0);
    ~MainDebugWindow();

    void init();

    friend class Frame;
    friend class DenseKitchen;

private slots:
    void on_runButton_clicked();
    void on_pauseButton_clicked();
    void on_stepButton_clicked();

private:
    Ui::MainDebugWindow *ui;
    DenseKitchen program;
    std::map<QString,DebugView> debugViews;

    void setUpDebugViewWindows();
};

#endif // MAINDEBUGWINDOW_HPP
