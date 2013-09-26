cmake_minimum_required (VERSION 2.8.1)

enable_testing()

add_executable(runTests
    main.cpp
)

target_link_libraries(runTests gtest gtest_main)

add_test(
  	NAME runTests
    COMMAND runTests
)