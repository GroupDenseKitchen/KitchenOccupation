#include "DenseKitchen.h"

int main()
{
	DenseKitchen program;
	program.readConfig("config.?");
	program.run();
	return 0;
}