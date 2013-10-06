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
    DenseKitchen program;
    program.init();
    program.readConfig("file.yml");

    setUpDebugViewWindows();
}

void MainDebugWindow::on_runButton_clicked()
{
    qDebug() << "Run button clicked!";
}

void MainDebugWindow::on_pauseButton_clicked()
{
    qDebug() << "Pause button clicked!";
}

void MainDebugWindow::on_stepButton_clicked()
{
    qDebug() << "Step button clicked!";
}

void MainDebugWindow:: setUpDebugViewWindows()
{

    Frame lastFrame;

    cv::Mat dummyImage;
    lastFrame.addHistory("First",dummyImage);
    lastFrame.addHistory("Second",dummyImage);
    lastFrame.addHistory("Third",dummyImage);
    lastFrame.addHistory("Fourth",dummyImage);
//TODO: Continue developing from here
    ui_->chooseDebugView->clear();
       std::map<QString,int>::iterator debugView = debugViews.begin();
       for(debugView; debugView != debugViews.end(); ++debugView)
       {
           ui->chooseDebugView->addItem(debugView->first);
           qDebug() << debugView->first;
       }

       debugViewWindows.clear();
       std::map<QString,int>::iterator it = debugViews.begin();
       for(it; it != debugViews.end(); ++it)
       {
           debugViewWindows.insert(std::pair<QString, DebugView *>(it->first,NULL));
       }

}
