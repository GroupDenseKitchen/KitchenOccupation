#include "DenseKitchen.h"

int main()
{
	DenseKitchen program;
	program.readConfig("config.yml");
	program.run();
	return 0;
}