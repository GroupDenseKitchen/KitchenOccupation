#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

namespace network{

class NetworkModule{
public:
	NetworkModule();
	~NetworkModule();

	bool hasNewFrame();
	Frame dequeFrame();
private:

};

}
#endif