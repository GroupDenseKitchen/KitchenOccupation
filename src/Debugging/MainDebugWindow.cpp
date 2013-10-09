#include "MainDebugWindow.hpp"
#include "ui_MainDebugWindow.h"
#include <QtWidgets>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include "../Utilities/Frame.hpp"


MainDebugWindow::MainDebugWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDebugWindow)
{
    ui->setupUi(this);
}

MainDebugWindow::~MainDebugWindow()
{
    delete ui;
}


void MainDebugWindow:: init()
{
    program.readConfig("file.yml");
    program.init();

    setUpDebugViewWindows();
    isRunningProgram = false;
}

void MainDebugWindow::on_runButton_clicked()
{
    qDebug() << "Run button clicked!";
    isRunningProgram = true;
    while(isRunningProgram){
        isRunningProgram = program.singleIteration();
        updateDebugViews();
    }
}

void MainDebugWindow::on_pauseButton_clicked()
{
    qDebug() << "Pause button clicked!";
    isRunningProgram = false;
}

void MainDebugWindow::on_stepButton_clicked()
{
    qDebug() << "Step button clicked!";
    program.singleIteration();
    updateDebugViews();
    isRunningProgram = false;
}

void MainDebugWindow:: setUpDebugViewWindows()
{

    //Create some dummy data to use for developing
    //TODO: This is where we should get the names of existing process steps
    //from DenseKitchen
    Frame lastFrame;
    cv::Mat dummyImage;
    lastFrame.addHistory("First",dummyImage);
    lastFrame.addHistory("Second",dummyImage);
    lastFrame.addHistory("Third",dummyImage);
    lastFrame.addHistory("Fourth",dummyImage);
    //End dummy stuff

    ui->debugViewChooser->clear();
    ui->debugViewChooser->addItem("<none>");
    debugViews.clear();
    std::map<std::string, cv::Mat> processSteps = lastFrame.processHistory;
    std::map<std::string, cv::Mat>::iterator processStep = processSteps.begin();
    for(processStep; processStep != processSteps.end(); ++processStep) {
        QString processStepName = processStep->first.c_str();
        ui->debugViewChooser->addItem(processStepName);
        debugViews.insert(std::pair<QString, DebugView *>(processStepName,NULL));

        qDebug() << processStepName;
    }
}

void MainDebugWindow::on_debugViewChooser_currentIndexChanged(const QString &chosenView)
{
    if (chosenView != "<none>") {
        DebugView * oldWindow = debugViews.find(chosenView)->second;
        if (oldWindow != NULL) {
            debugViews.find(chosenView)->second->close();
        }
        DebugView * newWindow = new DebugView(this);
        newWindow->init(chosenView);
        debugViews.find(chosenView)->second = newWindow;
        newWindow->show();
    }
    updateDebugViews();
}

void MainDebugWindow:: updateDebugViews()
{
    //TODO:Make this function read from processHistory in program

    cv::Mat image = cv::Mat::ones(200,200, CV_8UC1);
    uint multiplier = 1;
    std::map<QString, DebugView *>::iterator debugView = debugViews.begin();
    for(debugView; debugView != debugViews.end(); ++debugView) {
        image = image*multiplier;
        if (debugView->second != NULL) {
            debugView->second->showImage(image);
        }
        multiplier += 30;
        qDebug() << debugView->first << ", multiplier =" << multiplier;
    }
}
