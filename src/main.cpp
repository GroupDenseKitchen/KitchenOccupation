#include "DenseKitchen.h"

int main()
{
	DenseKitchen program;
	program.readConfig("config.yml");
	program.start();
	return 0;
}