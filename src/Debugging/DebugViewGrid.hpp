#ifndef DEBUGVIEWGRID_H
#define DEBUGVIEWGRID_H

#include <QWidget>
#include <QCloseEvent>
#include "DebugViewWidget.hpp"


namespace Ui {
class DebugViewGrid;
}

/*!
 * \brief   The DebugViewGrid class is a container for \ref DebugViewWidget "DebugViewWidgets".
 * \details Processs steps that are selected and poped from the \ref MainDebugWindow will end up here.
 *          An arbitrarilly number of widgets can be pop to the grid and these are automatically arranged
 *          and resized to fit the current grid size.
 */
class DebugViewGrid : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief        Constructor
     */
    explicit DebugViewGrid(QWidget *parent = 0);

    /*!
     * \brief   Destructor
     */
    ~DebugViewGrid();

    /*!
     * \brief          Initialize takes the desired number of columns in the \ref DebugViewGrid and sets up the auto-adaption.
     */
    void initialize(int nColumns);

    /*!
     * \brief        Inserts an \ref DebugViewWidget in the grid.
     */
    void addWidget(DebugViewWidget* widget);

    /*!
     * \brief   Removes everything that is currently in the grid.
     */
    void clearGrid();

private:
    Ui::DebugViewGrid *ui;

    int maxColumns;
    int row, column;

    std::vector<DebugViewWidget*> gridWidgets;

    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // DEBUGVIEWGRID_H
