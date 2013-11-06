#include "MainConfigurationWindow.hpp"
#include "ui_MainConfigurationWindow.h"

MainConfigurationWindow::MainConfigurationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainConfigurationWindow)
{
    ui->setupUi(this);
    //setWindowModality(Qt::ApplicationModal);
    setWindowFlags( Qt::WindowStaysOnTopHint );
}

MainConfigurationWindow::~MainConfigurationWindow()
{
    delete ui;
}

void MainConfigurationWindow::closeEvent(QCloseEvent * e)
{
    e->accept();
}

