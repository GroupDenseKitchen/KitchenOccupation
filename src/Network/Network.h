#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include "../utilities/include.h"

namespace network{

class Network{
public:
	Network();
	~Network();

    void start(void);
	bool hasNewFrame(void);
	Frame dequeFrame(void);
private:

};

}
#endif