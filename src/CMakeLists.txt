cmake_minimum_required (VERSION 2.8.1)
#Dense kitchen project
project(dense_kitchen)

# is TRUE on Apple OS X
MESSAGE( STATUS "APPLE: " ${APPLE} )
# is TRUE when using the MinGW compiler in Windows
MESSAGE( STATUS "MINGW: " ${MINGW} )


#something apple
IF(APPLE)
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++")
ENDIF()

IF(MINGW)
    #something windows
ENDIF()

#something windows

#magic QT preprocessor?

#load external libs

#load project libs
add_subdirectory(Configuration)
add_subdirectory(Debugging)
add_subdirectory(ImageProcessing)
add_subdirectory(Network)
add_subdirectory(Analytics)
add_subdirectory(Utilities)


#binaries
add_executable(dense_kitchen main.cpp DenseKitchen.cpp)
add_executable(debug_kitchen debugGui.cpp DebugKitchen.cpp)

#link debug gui to libraries
target_link_libraries (debug_kitchen 
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
target_link_libraries (dense_kitchen 
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