#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include "../utilities/utilities.hpp"
#include "../utilities/Frame.hpp"

namespace network{

class Network{
public:
	Network();
	~Network();

    void start(bool loadFromFile);
	bool hasNewFrame(void);
	Frame dequeFrame(void);
private:

};

}
#endif
