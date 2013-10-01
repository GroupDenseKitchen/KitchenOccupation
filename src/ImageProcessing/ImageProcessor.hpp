#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"

namespace image_processing
{
class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();
    void processFrame(FrameList frames);
private:
};
}

#endif
