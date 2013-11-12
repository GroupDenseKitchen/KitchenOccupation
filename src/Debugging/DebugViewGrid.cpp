#include "DebugViewGrid.hpp"
#include "ui_DebugViewGrid.h"

DebugViewGrid::DebugViewGrid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugViewGrid)
{
    ui->setupUi(this);
    setWindowTitle("The Grid");
}

DebugViewGrid::~DebugViewGrid()
{
    delete ui;
}

void DebugViewGrid::init(int columns)
{
    maxColumns = columns - 1;
    row = 0;
    column = 0;
}

void DebugViewGrid::addWidget(DebugViewWidget* widget)
{
    ui->gridLayout->addWidget(widget, row, column);
    gridWidgets.push_back(widget);

    column++;
    if(column > maxColumns){
        column = 0;
        row++;
    }
}

void DebugViewGrid::clearGrid()
{
    for(unsigned int i = 0; i < gridWidgets.size(); i++){
        gridWidgets[i]->deleteLater();
        ui->gridLayout->removeWidget(gridWidgets[i]);
    }

    gridWidgets.clear();

    row = 0;
    column = 0;
}

void DebugViewGrid::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void DebugViewGrid::closeEvent(QCloseEvent *e)
{
    e->accept();
}

//void DebugViewGrid::resizeEvent(QResizeEvent *e){
//    e->accept();
//}
