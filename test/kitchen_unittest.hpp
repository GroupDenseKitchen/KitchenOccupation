#include "gtest/gtest.h"
#include "../src/DenseKitchen.h"

namespace {

// Tests that the Foo::Bar() method does Abc.
TEST(KitchenTest, ConstructorDoesNotCrash) {
  bool initNoException = true;
  try{
    DenseKitchen dk = DenseKitchen();
  }catch(const E &e){
    //got exception, shit is broken.
    initNoException = false;
  }

  ASSERT_TRUE(initNoException);
}
/*
TEST(KitchenTest, ReadConfigDoesNotCrash) {
  
  EXPECT_EQ(0, f.Bar(input_filepath, output_filepath));
}

TEST(KitchenTest, ReadInvalidConfigCrash) {
  
  EXPECT_EQ(0, f.Bar(input_filepath, output_filepath));
}
*/

}  // namespace