#include "DebugView.hpp"
#include "ui_DebugView.h"

DebugView::DebugView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugView)
{
    ui->setupUi(this);
}

DebugView::~DebugView()
{
    delete ui;
}

void DebugView:: setName(QString newName)
{
    name = newName;
    ui->label->setText(newName);
}
