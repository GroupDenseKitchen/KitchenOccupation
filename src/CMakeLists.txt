CMAKE_MINIMUM_REQUIRED (VERSION 2.8.1)
#Dense kitchen project
PROJECT(dense_kitchen)

# is TRUE on Apple OS X
MESSAGE( STATUS "APPLE: " ${APPLE} )
# is TRUE when using the MinGW compiler in Windows
MESSAGE( STATUS "MINGW: " ${MINGW} )


#something apple
IF(APPLE)
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
ENDIF()

IF(MINGW)
    #something windows
ENDIF()

#something windows

#magic QT preprocessor?

#load external libs

#load project libs
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Debugging)
ADD_SUBDIRECTORY(ImageProcessing)
ADD_SUBDIRECTORY(Network)
ADD_SUBDIRECTORY(Analytics)
ADD_SUBDIRECTORY(Utilities)


#binaries
ADD_EXECUTABLE(dense_kitchen main.cpp DenseKitchen.cpp)
ADD_EXECUTABLE(debug_kitchen debugGui.cpp DebugKitchen.cpp)

#link debug gui to libraries
TARGET_LINK_LIBRARIES (debug_kitchen 
    Configuration 
    Debugging 
    ImageProcessing 
    Network 
    Statistics 
    Utilities
    opencv_core 
    opencv_highgui 
    opencv_imgproc
    opencv_video) 

#link to libraries
TARGET_LINK_LIBRARIES (dense_kitchen 
    Configuration 
    Debugging 
    ImageProcessing 
    Network 
    Statistics 
    Utilities
    opencv_core 
    opencv_highgui 
    opencv_imgproc
    opencv_video) 