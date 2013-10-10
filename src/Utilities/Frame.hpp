#ifndef FRAME_H
#define FRAME_H

#include <opencv2/core/core.hpp>

#include <map>

/*!
 *  \brief     A container of a snap shot of a discrete time step.
 *  \details   Contain raw images from all camreas taken at the same time.
 *             Contain persons seen.
 *             Contain all intermediate debuging data.
 *  \version   0.1
 *  \date      2013-10-07
 */
class Frame
{
public:

    /*!
       \brief   Constructor.
    */
	Frame();

    /*!
       \brief   Destructor.
    */
	~Frame();

    /*!
       \brief        Add a raw image (done by the image provider).
       \param image  A camera image.
    */
	void addRawImage(cv::Mat image);

    /*!
       \brief        Add a raw image (done by the image provider).
       \param tag    A string with a very short descriptive label for the image.
       \param image  A camera image.
    */
    void addHistory(std::string tag, cv::Mat image);

private:
    std::vector<cv::Mat> rawImages;
    std::map<std::string,cv::Mat> processHistory;

    // Here be dragons
};

#endif
