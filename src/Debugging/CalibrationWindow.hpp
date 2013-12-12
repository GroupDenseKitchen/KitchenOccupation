#ifndef CALIBRATIONWINDOW_HPP
#define CALIBRATIONWINDOW_HPP

#include "../DenseKitchen.hpp"
#include "../Utilities/Frame.hpp"

#include <QWidget>
#include <QCloseEvent>

#include <opencv2/core/core.hpp>


namespace Ui {
class CalibrationWindow;
}

/*!
 * \brief   The CalibrationWindow class is a window where calibrates the threshold level for the system.
 * \details This threshold level is used to adjust the system for the current installation height of the camera.
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
     * \brief initialize sets up the \ref CalibrationWindow
     * \param mainProgram is a pointer to the main program.
     */
    void initialize(DenseKitchen* mainProgram);

public slots:
    /*!
     * \brief updateWindow recievs a \ref Frame and updates the windows accordingly.
     */
    void updateWindow(Frame currentFrame);

private:
    Ui::CalibrationWindow *ui;
    DenseKitchen* mainProgram;
    std::string filePath;

    void storeImageLocally(const cv::Mat &image);

    void thresholdImage();
    void calculateHistogram();
    void updateGUIImages();

    cv::Mat matImage;
    cv::Mat thresholdedImage;
    cv::Mat histogramImage;
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
