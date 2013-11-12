#ifndef FOREGROUND_REGION_EXTRACTOR_TWO_HPP
#define FOREGROUND_REGION_EXTRACTOR_TWO_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/Algorithm.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>


namespace image_processing
{
class ForegroundRegionExtractorTwo : public Algorithm
{
public:
	ForegroundRegionExtractorTwo();
	~ForegroundRegionExtractorTwo();
	
	/*!
       \brief   Initialize the algorithm.
       \details Returns false if initialization fails,
                e.g. if a required variable is not set in the config file.
    */
	bool initialize(configuration::ConfigurationManager &settings) override;

	/*!
       \brief   Performs the foreground modulation.
    */
	void process(FrameList &frames) override;

private:
	//do the mean shift segmentation
    cv::Mat meanShiftSegmentation(cv::Mat raw);
	cv::Mat floodFill(cv::Mat img);

	cv::Mat betterSegmented;
};
}

#endif
