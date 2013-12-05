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

class MainConfigurationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainConfigurationWindow(QWidget *parent = 0);
    ~MainConfigurationWindow();

    /*!
     * \brief init
     * \param _mainProgram
     * \param _filepath
     */
    void init(DenseKitchen* _mainProgram , std::string _filepath);

public slots:
    /*!
     * \brief updateWindow
     * \param currentFrame
     */
    void updateWindow(Frame currentFrame);

private:
    Ui::MainConfigurationWindow *ui;
    DenseKitchen* mainProgram;
    std::string filePath;

    void storeImageLocally(const cv::Mat &image);

    void drawPolygon(QVector<cv::Point> polygon, cv::Scalar color);
    void drawPolygons(std::string maskType, QVector<QVector<cv::Point>> polygons, cv::Scalar color);
    void drawPolygonsToMat(cv::Mat targetMat, const cv::Point** polygonPtrPtr, int numberOfPoints[], cv::Scalar color);
    void drawPolygonsToMasks();
    void drawCheckPointCircles();
    void sendMasksToFrameList();

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
    void on_addAsCheckpointButton_clicked();
    void on_circleCheckBox_clicked(bool checked);

};

#endif // MAINCONFIGURATIONWINDOW_HPP
