#include "DenseKitchen.hpp"
#include <QtCore>

int main()
{
	DenseKitchen program;
	program.init();
	program.readConfig("file.yml");
	return 0;
}
