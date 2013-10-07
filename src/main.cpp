#include "DenseKitchen.hpp"
#include <QtCore>
#include "Debugging/Logger.hpp"
#include <iostream>

int main()
{

	DenseKitchen program;
	program.init();
    program.readConfig("file.yml");
    std::cin.get();
	return 0;
}
