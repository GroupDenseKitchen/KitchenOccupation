#ifndef MAINCONFIGURATIONWINDOW_HPP
#define MAINCONFIGURATIONWINDOW_HPP

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class MainConfigurationWindow;
}

class MainConfigurationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainConfigurationWindow(QWidget *parent = 0);
    ~MainConfigurationWindow();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::MainConfigurationWindow *ui;
};

#endif // MAINCONFIGURATIONWINDOW_HPP
