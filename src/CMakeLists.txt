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
    # OpenCV/include and Opencv/bin should be located two folders above your project
    SET(OpenCV_INCLUDE_DIR "../../include")
    INCLUDE_DIRECTORIES(OpenCV_INCLUDE_DIR)
    SET(OpenCV_LIBS_DIR "../../bin")
    LINK_DIRECTORIES(OpenCV_LIBS_DIR)
ENDIF()

#magic QT preprocessor?
SET(CMAKE_AUTOMOC TRUE)

#load external libs
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

#load project libs
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Debugging)
ADD_SUBDIRECTORY(ImageProcessing)
ADD_SUBDIRECTORY(Network)
ADD_SUBDIRECTORY(Analytics)
ADD_SUBDIRECTORY(Utilities)

#binaries
ADD_EXECUTABLE(dense_kitchen main.cpp DenseKitchen.cpp debugGui.cpp)

#Activate Qt for project
qt5_use_modules(dense_kitchen Widgets)

#link to libraries
TARGET_LINK_LIBRARIES (dense_kitchen 
    Configuration 
    Debugging 
    ImageProcessing 
    Network 
    Statistics 
    Utilities
    ${OpenCV_LIBS})
