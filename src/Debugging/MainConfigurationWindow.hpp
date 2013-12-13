#ifndef MAINCONFIGURATIONWINDOW_HPP
#define MAINCONFIGURATIONWINDOW_HPP

#include "../DenseKitchen.hpp"
#include "../Utilities/Frame.hpp"

#include <QWidget>
#include <QCloseEvent>

#include <opencv2/core/core.hpp>


namespace Ui {
class MainConfigurationWindow;
}

/*!
 * \brief   The MainConfigurationWindow class is a window where you can cnfigure
 *          different settings in the system.
 * \details Especially masks used to define doors and
 *          exceptions for a camera. The door mask is used to check if a person is
 *          currently in a doorway and the exception mask is used to speed up and harden
 *          the system by defining ares that are not interesrting for the system.
 */
class MainConfigurationWindow : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief   Constructor
     */
    explicit MainConfigurationWindow(QWidget *parent = 0);

    /*!
     * \brief   Destructor
     */
    ~MainConfigurationWindow();

    /*!
     * \brief initialize sets up the \ref MainConfigurationWindow
     * \param mainProgram is a poninter to the main program.
     * \param masksConfigFile contains the coordinates for the masks used by the system.
     */
    void initialize(DenseKitchen* mainProgram , std::string masksConfigFile);


public slots:

    /*!
     * \brief updateWindow recieves a frame and updates the content in the \ref MainConfigurationWindow.
     */
    void updateWindow(Frame currentFrame);

private:
    Ui::MainConfigurationWindow *ui;
    DenseKitchen* mainProgram;
    std::string masksConfigFile;

    void storeImageLocally(const cv::Mat &image);

    void drawPolygon(QVector<cv::Point> polygon, cv::Scalar color);
    void drawPolygons(std::string maskType, QVector<QVector<cv::Point>> polygons, cv::Scalar color);
    void drawPolygonsToMat(cv::Mat targetMat, const cv::Point** polygonPtrPtr, int numberOfPoints[], cv::Scalar color);
    void drawPolygonsToMasks();
    void drawCheckPointCircles();


    QVector<cv::Point> polygon;
    QVector<QVector<cv::Point>> doorPolygons;
    QVector<QVector<cv::Point>> exclusionPolygons;
    bool drawAsCircle, isDrawingCircle;
    cv::Point circleCenter;
    int circleRadius;

    void updateGUIImages();

    cv::Mat matImage;
    cv::Mat imageWithMask;
    cv::Mat doorMask;
    cv::Mat exclusionMask;
    cv::Mat checkpointMaskSmall;
    cv::Mat checkpointMaskMedium;
    cv::Mat checkpointMaskLarge;
    cv::Mat resizedImage;
    QImage qImage;

    void loadMaskFromFile();
    void sendMasksToFrameList();
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
    void on_newPolygonButton_clicked();
    void on_addAsDoorButton_clicked();
    void on_addAsExclusionButton_clicked();
    void on_saveMasksButton_clicked();
    void on_loadMasksButton_clicked();
    void on_clearAllButton_clicked();
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
    void on_circleCheckBox_clicked(bool checked);

};

#endif // MAINCONFIGURATIONWINDOW_HPP
