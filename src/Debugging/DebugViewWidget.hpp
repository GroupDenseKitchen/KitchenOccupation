#ifndef DEBUGVIEWWIDGET_HPP
#define DEBUGVIEWWIDGET_HPP

#include "CvImageWidget.hpp"
#include "../Utilities/Frame.hpp"
#include <QWidget>
#include <opencv2/core/core.hpp>


namespace Ui {
class DebugViewWidget;
}

/*!
 * \brief   The DebugViewWidget class if simply a container for a \ref cv::Mat representing a certain
 *          step in the preocess chain.
 */
class DebugViewWidget : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Constructor
     */
    explicit DebugViewWidget(QWidget *parent = 0);

    /*!
     * \brief Destructor
     */
    ~DebugViewWidget();

    /*!
     * \brief                 Sets up the widget, adding a label with the represented preocesses step and camera.
     * \param processStepName is the name of a debug image set by the developer.
     * \param camNumber       is number of the camera that the process step belongs to.
     */
    void initialize(const std::string processStepName, int camNumber);

    /*!
     * \brief   Converts the \ref cv::Mat into a \ref QImage and displays it.
     */
    void showImage();

    /*!
     * \brief   Returns the process step and camera of the widget.
     */
    std::string getIdentifier();

public slots:
    /*!
     * \brief updateView receivs a \ref Frame, stores it in the widget and displays the new one.
     */
    void updateView( Frame );

signals:
    void aboutToClose(std::string);

private:
    Ui::DebugViewWidget *ui;
    std::string TAG;
    std::string processStep;
    uint cameraNumber;

    cv::Mat matImage, resizedImage;
    QImage qImage;

    void storeImage(const cv::Mat& image);
    void adaptImageToWidget();

    int windowWidth;
    int windowHeight;
    float windowRatio;

    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // DEBUGVIEWWIDGET_HPP
