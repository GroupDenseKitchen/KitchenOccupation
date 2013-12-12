#ifndef CALIBRATIONWINDOW_HPP
#define CALIBRATIONWINDOW_HPP

#include "../DenseKitchen.hpp"
#include "../Utilities/Frame.hpp"

#include <QWidget>
#include <QCloseEvent>

#include <opencv2/core/core.hpp>

/*!
 * \brief   TODO
 * \details TODO
 */
namespace Ui {
class CalibrationWindow;
}

/*!
 * \brief   The CalibrationWindow class is a window where you can cnfigure
 *          different settings in the system. Especially masks used to define doors and
 *          exceptions for a camera. The door mask is used to check if a person is
 *          currently in a doorway and the exception mask is used to speed up and harden
 *          the system by defining ares that are not interesrting for the system.
 * \details TODO
 */
class CalibrationWindow : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief   Constructor
     */
    explicit CalibrationWindow(QWidget *parent = 0);

    /*!
     * \brief   Destructor
     */
    ~CalibrationWindow();

    /*!
     * \brief             initialize sets up the \ref CalibrationWindow
     * \details           TODO
     * \param mainProgram TODO
     * \param filepath    TODO
     */
    void initialize(DenseKitchen* mainProgram , std::string filepath);

public slots:
    /*!
     * \brief              TODO
     * \details            TODO
     * \param currentFrame TODO
     */
    void updateWindow(Frame currentFrame);

private:
    Ui::CalibrationWindow *ui;
    DenseKitchen* mainProgram;
    std::string filePath;

    void storeImageLocally(const cv::Mat &image);

    void thresholdImage();
    void updateGUIImages();

    cv::Mat matImage;
    cv::Mat thresholdedImage;
    cv::Mat resizedImage;
    QImage qImage;

    int thresholdValue;

    void loadMaskFromFile();
    void storeMask(QVector<QVector<cv::Point> > polygons, std::string nodeName);
    bool readMasks(QVector<QVector<cv::Point> > &polygons, std::string nodeName);

    cv::FileStorage configFile;

    // Events
    void closeEvent(QCloseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:

    void on_cancelButton_clicked();
    void on_applyButton_clicked();

    void on_thresholdSlider_valueChanged(int value);
};

#endif // CALIBRATIONWINDOW_HPP
