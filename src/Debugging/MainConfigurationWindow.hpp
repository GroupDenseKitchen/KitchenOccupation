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

    void init(DenseKitchen* _mainProgram , std::string _filepath);
    void applyChanges();

    DenseKitchen* mainProgram;
    std::string filePath;

    void showImage();
    void storeImage(const cv::Mat &image);

    void overlayMask();
    void drawPolygons(std::string maskType, QVector<QVector<cv::Point>> polygons, cv::Scalar color);
    void drawPolygon(QVector<cv::Point> polygon, cv::Scalar color);
    void polygonDrawer(cv::Mat targetMat, const cv::Point** polygonPtrPtr, int numberOfPoints[], cv::Scalar color);
    void drawCheckPointCircles();

    cv::Mat matImage, imageWithMask, doorMask, exclusionMask, checkpointMaskSmall, checkpointMaskMedium, checkpointMaskLarge, resizedImage;
    QImage qImage;

    bool drawAsCircle, isDrawingCircle;
    cv::Point circleCenter;
    int circleRadius;

    QVector<QVector<cv::Point>> doorPolygons;
    QVector<QVector<cv::Point>> exclusionPolygons;
    QVector<cv::Point> polygon;

    void loadMaskFromFile();
    cv::FileStorage configFile;
    void storeMask(QVector<QVector<cv::Point> > polygons, std::string nodeName);
    bool readMasks(QVector<QVector<cv::Point> > &polygons, std::string nodeName);

public slots:
    void updateWindow(Frame currentFrame);

protected:
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

private:
    Ui::MainConfigurationWindow *ui;
};

#endif // MAINCONFIGURATIONWINDOW_HPP
