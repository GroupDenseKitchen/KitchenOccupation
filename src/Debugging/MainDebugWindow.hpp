#ifndef MAINDEBUGWINDOW_HPP
#define MAINDEBUGWINDOW_HPP

#include <QMainWindow>
#include <QtGui>
#include <QString>
#include <QTimer>
#include <QImage>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>

#include <map>
#include <atomic>

#include "../DenseKitchen.hpp"
#include "DebugView.hpp"
#include "DebugViewWidget.hpp"

#include <opencv2/core/core.hpp>

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

    int getCurrentCameraIndex() const;
    void setCurrentCameraIndex(int value);

private slots:
    void updateGUI();

    void cameraSelctionUpdate(QModelIndex,QModelIndex);

    void on_runButton_clicked();
    void on_pauseButton_clicked();
    void on_stepForwardButton_clicked();
    void on_stepBackwardButton_clicked();

    void on_tagFilterLineEdit_textEdited(const QString &arg1);
    void on_timeFilterLineEdit_textEdited(const QString &arg1);
    void on_messageFilterLineEdit_textEdited(const QString &arg1);
    void on_callingFuncFilterLineEdit_textEdited(const QString &arg1);
    void on_lineNumberFilterLineEdit_textEdited(const QString &arg1);

    void on_popWindowButton_clicked();
    void on_autoAdaptProfilerCheckBox_clicked(bool checked);

    void on_autoAdaptLogCheckBox_clicked(bool checked);

    void on_expandDepthSpinBox_valueChanged(int arg1);

public slots:
    void removeDebugViewWidget(std::string identifier);

signals:
    void updateDebugViews( Frame currentFrame);

private:
    Ui::MainDebugWindow *ui;
    DenseKitchen program;
    std::map<std::string,DebugViewWidget*> debugViews;
    bool isRunning;
    std::string configPath;

    QTreeView* cameraTree;
    QStandardItemModel* cameraItemModel;
    QItemSelectionModel* cameraSelection;

    int currentCameraIndex;
    std::string currentKey;
    bool popAllWindows;

    QTreeView* logTree;
    QStandardItemModel* logItemModel;
    QSortFilterProxyModel* logProxyModel;

    void updateLog();
    bool autoAdaptLog;

    QTreeView* profilerTree;
    QStandardItemModel* profilerItemModel;
    QSortFilterProxyModel* profilerProxyModel;

    void updateProfiler();
    bool autoAdaptProfiler;
    int profilerExpandDepth;

    void adaptColumnsToContent(QTreeView* tree, QStandardItemModel* model);

    QImage qImage;

    QTimer* timer;
    int timerDelay;

    cv::FileStorage configFile;
    void configureGUI();
    bool readConfig(std::string filePath);
    void generateConfig(std::string filePath);
    void updateGuiComponents();
    void clearLogObject();
    void updateProfilerChildren(QStandardItem *parentItem, std::list<debugging::ProfilerEntry> children);
    void popWindow(std::string stepKey, int cameraIndex);

    void closeEvent(QCloseEvent *);
};

#endif // MAINDEBUGWINDOW_HPP
