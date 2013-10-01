#include "../Utilities/FrameList.h"

namespace image_processing{

class AbstractAlgorithm{
public:
	virtual void process(const FrameList frames);
private:
};

}