#ifndef DEBUGVIEWGRID_H
#define DEBUGVIEWGRID_H

#include <QWidget>
#include <QCloseEvent>
#include "DebugViewWidget.hpp"

/*!
 * \brief   TODO
 * \details TODO
 */
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
     * \param parent TODO
     */
    explicit DebugViewGrid(QWidget *parent = 0);

    /*!
     * \brief   Destructor
     */
    ~DebugViewGrid();

    /*!
     * \brief          Initialize takes the desired number of columns and sets up the auto-adaption.
     * \details        TODO
     * \param nColumns TODO
     */
    void initialize(int nColumns);

    /*!
     * \brief        Inserts an \ref DebugViewWidget in the grid.
     * \details      TODO
     * \param widget TODO
     */
    void addWidget(DebugViewWidget* widget);

    /*!
     * \brief   Removes everything that is currently in the grid.
     * \details TODO
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
