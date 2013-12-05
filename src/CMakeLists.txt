CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)
# Dense kitchen project
PROJECT(dense_kitchen)

# is TRUE on Apple OS X
MESSAGE( STATUS "APPLE: " ${APPLE} )
# is TRUE when using the MinGW compiler in Windows
MESSAGE( STATUS "MINGW: " ${MINGW} )

IF(APPLE)
    # Apple specific
    INCLUDE_DIRECTORIES(/usr/local/include/)
    INCLUDE_DIRECTORIES(/usr/local/include/libfreenect)
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
ENDIF()

IF(WIN32)
    # Windows specific
    IF(MINGW)
        SET (CMAKE_CXX_FLAGS "-std=c++11 -Wall")
    ENDIF()

    IF(MSVC)
        MESSAGE( STATUS "ENV_OPENCV_MSVC2012_X64_INSTALL_DIR: " $ENV{OPENCV_MSVC2012_X64_INSTALL_DIR})
        MESSAGE( STATUS "ENV_OPENNI2_INCLUDE: " $ENV{OPENNI2_INCLUDE64} )
        MESSAGE( STATUS "ENV_OPENNI2_LIB: " $ENV{OPENNI2_LIB64} )

        FIND_LIBRARY( OPENNI2_LIBS
            NAMES OpenNI2
            HINTS $ENV{OPENNI2_LIB64}
            PATH_SUFFIXES lib lib64)
        FIND_PATH( OPENNI2_INCLUDE_DIR OpenNI.h
            HINTS $ENV{OPENNI2_INCLUDE64}
            PATH_SUFFIXES openni2 ni2)

        MESSAGE( STATUS "OPENNI2_INCLUDE_DIR: " ${OPENNI2_INCLUDE_DIR} )
        MESSAGE( STATUS "OPENNI2_LIBS: " ${OPENNI2_LIBS} )

        add_definitions(-D_ITERATOR_DEBUG_LEVEL=1) # Remove itterator assert
        cmake_policy(SET CMP0020 NEW) # Remove cmake warnings
    ENDIF()
ENDIF()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#move the configurationfile to the build dir
CONFIGURE_FILE(../conf/dense_conf.yml dense_conf.yml COPYONLY)
CONFIGURE_FILE(../conf/guiConfig.yml guiConfig.yml COPYONLY)

# Load external libraries
find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)
find_package(OpenCV REQUIRED HINTS $ENV{OPENCV_MSVC2012_X64_INSTALL_DIR})
MESSAGE( STATUS "OPENCV_INCLUDE_DIRS: " ${OpenCV_INCLUDE_DIRS} )
MESSAGE( STATUS "OPENCV_LIB_DIR: " ${OpenCV_LIB_DIR} )
#MESSAGE( STATUS "OPENCV_LIBS: " ${OpenCV_LIBS} )

# Include
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Network_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENNI2_INCLUDE_DIR})

# Load project libs
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Debugging)
ADD_SUBDIRECTORY(ImageProcessing)
ADD_SUBDIRECTORY(Network)
ADD_SUBDIRECTORY(Analytics)
ADD_SUBDIRECTORY(Utilities)
# ADD_SUBDIRECTORY(ExtLib)

# Binaries
ADD_EXECUTABLE(dense_kitchen main.cpp DenseKitchen.cpp)

# Link to libraries
TARGET_LINK_LIBRARIES (dense_kitchen
    Configuration
    Debugging
    ImageProcessing
    Network
    Statistics
    Utilities
    # ExtLib
    ${OpenCV_LIBS}
    ${OPENNI2_LIBS}
)

# Activate Qt for project
# Must be the last line in this file, else { FAIL }
qt5_use_modules(dense_kitchen Core Gui Widgets Network)
