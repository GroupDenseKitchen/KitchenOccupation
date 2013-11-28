#ifndef MAINDEBUGWINDOW_HPP
#define MAINDEBUGWINDOW_HPP

#include <QMainWindow>
#include <QtGui>
#include <QKeyEvent>
#include <QString>
#include <QTimer>
#include <QImage>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>

#include <map>

#include "../DenseKitchen.hpp"
#include "MainConfigurationWindow.hpp"
#include "DebugView.hpp"
#include "DebugViewWidget.hpp"
#include "DebugViewGrid.hpp"
#include "StereoCalibrationWidget.hpp"

#include <opencv2/core/core.hpp>

namespace Ui {
class MainDebugWindow;
}

/*!
 * \brief       The MainDebugWindow class is a debug interface to speed-up development
 *              and testing of image processing algorithms.
 * \version     0.1
 * \date        2013-10-18
 */
class MainDebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief MainDebugWindow
     * \param parent
     */
    explicit MainDebugWindow(QWidget *parent = 0);

    /*!
     * \brief Destructor
     */
    ~MainDebugWindow();

    /*!
     * \brief Initializes the GUI with values specified in
     *  guiConfig.yml
     */
    void init();

    /*!
     * \brief The GUI needs full access to DenseKitchen.
     */
    friend class DenseKitchen;

private slots:
    void updateGUI();

    void cameraSelectionUpdate(QModelIndex,QModelIndex);

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

    void on_configureButton_clicked();
    void on_actionClear_triggered();
    void on_actionSave_grid_configuration_triggered();

    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionRestart_triggered();
    void on_actionConfigure_triggered();
    void on_actionStereoCalibrate_triggered();

public slots:
    void removeDebugViewWidget(std::string identifier);

signals:
    void updateDebugViews( Frame currentFrame);

private:
    Ui::MainDebugWindow *ui;
    DenseKitchen* program;
    MainConfigurationWindow* configWindow;

    StereoCalibrationWidget* stereoCalibrationWidget;

    std::map<std::string,DebugViewWidget*> debugViews;
    bool isRunning;
    std::string guiConfigPath, mainConfigPath;

    DebugViewGrid* debugViewGrid;
    std::vector<int> presetCameraNumber;
    std::vector<std::string> presetStepName;

    QTreeView* cameraTree;
    QStandardItemModel* cameraItemModel;
    QItemSelectionModel* cameraSelection;

    void updateCameraSelector();
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
    void restart();

    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // MAINDEBUGWINDOW_HPP
