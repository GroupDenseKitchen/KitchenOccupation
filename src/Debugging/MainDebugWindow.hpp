#ifndef MAINDEBUGWINDOW_HPP
#define MAINDEBUGWINDOW_HPP

#include <QMainWindow>
#include <QString>

#include <map>
#include <atomic>

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

    friend class DenseKitchen;

private slots:
    void on_runButton_clicked();
    void on_pauseButton_clicked();
    void on_stepButton_clicked();

    void on_debugViewChooser_currentIndexChanged(const QString &arg1);

private:
    Ui::MainDebugWindow *ui;
    DenseKitchen program;
    std::map<QString,DebugView *> debugViews;
    std::atomic_bool isRunningProgram;


    void setUpDebugViewWindows();
    void updateDebugViews();
};

#endif // MAINDEBUGWINDOW_HPP
