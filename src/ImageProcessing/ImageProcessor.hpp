#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Abstractalgorithm.hpp"

namespace image_processing
{
class ImageProcessor : public AbstractAlgorithm
{
public:
	ImageProcessor();
	~ImageProcessor();

    void process(FrameList frames) override;
private:
};
}

#endif
