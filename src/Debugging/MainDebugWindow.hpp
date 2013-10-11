#ifndef MAINDEBUGWINDOW_HPP
#define MAINDEBUGWINDOW_HPP

#include <QMainWindow>
#include <QtGui>
#include <QString>
#include <QTimer>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QImage>

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
    void updateGUI();
    void cameraSelctionUpdate(QModelIndex,QModelIndex);

    void on_runButton_clicked();
    void on_pauseButton_clicked();
    void on_stepForwardButton_clicked();
    void on_stepBackwardButton_clicked();

private:
    Ui::MainDebugWindow *ui;
    DenseKitchen program;
    std::map<QString,DebugView *> debugViews;
    std::atomic_bool isRunning;

    QTreeView* cameraTree;
    QStandardItemModel* cameraItemModel;
    QItemSelectionModel* cameraSelection;

    int currentCameraIndex;
    std::string currentKey;

    QImage qImage;

    QTimer* timer;
    int timerDelay;

    void updateDebugViews();
};

#endif // MAINDEBUGWINDOW_HPP
