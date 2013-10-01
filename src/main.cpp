#include "DenseKitchen.hpp"

int main()
{
	DenseKitchen program;
	program.readConfig("config.yml");
	program.start();
	return 0;
}
