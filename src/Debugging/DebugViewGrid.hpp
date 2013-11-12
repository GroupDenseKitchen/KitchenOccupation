#ifndef DEBUGVIEWGRID_H
#define DEBUGVIEWGRID_H

#include <QWidget>
#include <QCloseEvent>
#include "DebugViewWidget.hpp"

namespace Ui {
class DebugViewGrid;
}

class DebugViewGrid : public QWidget
{
    Q_OBJECT

public:
    explicit DebugViewGrid(QWidget *parent = 0);
    ~DebugViewGrid();

    void init(int columns);
    void addWidget(DebugViewWidget* widget);
    void clearGrid();

private:
    Ui::DebugViewGrid *ui;

    int maxColumns;
    int row, column;

    std::vector<DebugViewWidget*> gridWidgets;

    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *e);
    //void resizeEvent(QResizeEvent *e);
};

#endif // DEBUGVIEWGRID_H
