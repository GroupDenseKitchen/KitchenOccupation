#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "../Utilities/include.h"
#include "../Utilities/FrameList.h"

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