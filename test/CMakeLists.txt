CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


#setup correct compiler settings
IF(APPLE)
	SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
ENDIF()

IF(MINGW)
	#windows stuff, no idea...
ENDIF()

#add the dense kitchen submodule libraries
ADD_SUBDIRECTORY(../src/Configuration "${CMAKE_CURRENT_BINARY_DIR}/Configuration")
ADD_SUBDIRECTORY(../src/Analytics "${CMAKE_CURRENT_BINARY_DIR}/Analytics")
ADD_SUBDIRECTORY(../src/Debugging "${CMAKE_CURRENT_BINARY_DIR}/Debugging")
ADD_SUBDIRECTORY(../src/Network "${CMAKE_CURRENT_BINARY_DIR}/Network")
ADD_SUBDIRECTORY(../src/Utilities "${CMAKE_CURRENT_BINARY_DIR}/Utilities")
ADD_SUBDIRECTORY(../src/ImageProcessing "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessing")

#check the getest dir for more cmake stuff
ADD_SUBDIRECTORY(gtest)
ENABLE_TESTING()

#include the gtest includes into the <file> scope for c++ (i think)
INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

ADD_EXECUTABLE(runUnitTests unit_test_main.cpp)

#link it all together

#first the gtest libs
TARGET_LINK_LIBRARIES(runUnitTests gtest gtest_main)

#then the project libs
TARGET_LINK_LIBRARIES(runUnitTests 
	Configuration Debugging ImageProcessing 
	Network Statistics Utilities)

#then the opencv libs
#this is bad and non-portable
TARGET_LINK_LIBRARIES(runUnitTests opencv_core opencv_highgui opencv_imgproc opencv_video)

ADD_TEST(SYSTEM_TEST system_test COMMAND runUnitTests)