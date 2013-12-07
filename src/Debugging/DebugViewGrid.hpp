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
     * \brief Constructor
     * \param parent
     */
    explicit DebugViewGrid(QWidget *parent = 0);

    /*!
     * \brief Destructor
     */
    ~DebugViewGrid();

    /*!
     * \brief initialize takes the desired number of columns and sets up the auto-adaption.
     * \param nColumns
     */
    void initialize(int nColumns);

    /*!
     * \brief addWidget inserts an \ref DebugViewWidget in the grid.
     * \param widget
     */
    void addWidget(DebugViewWidget* widget);

    /*!
     * \brief clearGrid removes everything that is currently in the grid.
     */
    void clearGrid();

private:
    Ui::DebugViewGrid *ui;

    int maxColumns;
    int row, column;

    std::vector<DebugViewWidget*> gridWidgets;

    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *e);
};

#endif // DEBUGVIEWGRID_H
