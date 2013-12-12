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
#include "DebugViewWidget.hpp"
#include "DebugViewGrid.hpp"
#include "CalibrationWindow.hpp"

#include <opencv2/core/core.hpp>

/*!
 * \brief   TODO
 * \details TODO
 */
namespace Ui {
class MainDebugWindow;
}

/*!
 * \brief   The MainDebugWindow class is a debug interface to speed up development,
 *          testing and validation of image processing algorithms.
 * \details TODO
 * \version 0.1
 * \date    2013-12-06
 */
class MainDebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief        Constructor
     * \param parent TODO
     */
    explicit MainDebugWindow(QWidget *parent = 0);

    /*!
     * \brief   Destructor
     */
    ~MainDebugWindow();

    /*!
     * \brief                Initializes the GUI with values specified in guiConfig.yml.
     * \details              TODO
     * \param mainConfigFile TODO
     * \param guiConfigFile  TODO
     */
    void init(std::string mainConfigFile, std::string guiConfigFile);

    /*!
     * \brief The GUI needs full access to DenseKitchen.
     */
    friend class DenseKitchen;

signals:
    /*!
     * \brief              updateDebugViews is used to send a fresh Frame to all sub-widgets and update their content.
     * \details            TODO
     * \param currentFrame TODO
     */
    void updateDebugViews( Frame currentFrame);

private:


    void configureGUI(string guiConfigFile);
    bool readConfig(std::string filePath);
    void generateConfig(std::string filePath);

    void popWindow(std::string stepKey, int cameraIndex);

    void updateGuiComponents();
    void updateCameraSelector();
    void updateProfiler();
    void updateProfilerChildren(QStandardItem *parentItem, std::list<debugging::ProfilerEntry> children);
    void updateLog();
    void clearLogObject();

    void adaptColumnsToContent(QTreeView* tree, QStandardItemModel* model);

    void restart();

private slots:
    void updateGUI();

    void cameraSelectionUpdate(QModelIndex,QModelIndex);
    void removeDebugViewWidget(std::string identifier);

private:
    Ui::MainDebugWindow *ui;

    DenseKitchen* program;

    DebugViewGrid* debugViewGrid;
    MainConfigurationWindow* configWindow;
    CalibrationWindow* calibrationWindow;
    std::map<std::string,DebugViewWidget*> debugViews;

    std::string guiConfigPath;
    std::string mainConfigPath;
    cv::FileStorage configFile;
    std::vector<int> presetCameraNumber;
    std::vector<std::string> presetStepName;

    bool isRunning;
    bool popAllWindows;

    QTreeView* cameraTree;
    QStandardItemModel* cameraItemModel;
    QItemSelectionModel* cameraSelection;
    int currentCameraIndex;
    std::string currentKey;

    QTreeView* logTree;
    QStandardItemModel* logItemModel;
    QSortFilterProxyModel* logProxyModel;

    bool autoAdaptLog;

    QTreeView* profilerTree;
    QStandardItemModel* profilerItemModel;
    QSortFilterProxyModel* profilerProxyModel;


    bool autoAdaptProfiler;
    int profilerExpandDepth;

    QTimer* timer;
    int timerDelay;

    bool isRecordToFiles;
    cv::VideoWriter * videoWriter;

    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);

private slots:

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
    void on_actionCalibrate_triggered();
};

#endif // MAINDEBUGWINDOW_HPP
