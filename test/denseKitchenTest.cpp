#include <gtest/gtest.h>

//This should be changed to onlt include DenseKitchen when
//this is possible to link in via cmake
//#include "DenseKitchen.h"

TEST(DenseKitchenTest, ConstructorDoesntFail)
{
    bool constructorFailed = false;
    try {
        //call DenseKitchen constructor
    }
    catch (int e) {
        constructorFailed = true;
    }

	ASSERT_FALSE( constructorFailed );
}

