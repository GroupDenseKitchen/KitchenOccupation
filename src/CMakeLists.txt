CMAKE_MINIMUM_REQUIRED (VERSION 2.8.7)
# Dense kitchen project
PROJECT(dense_kitchen)

OPTION(headless "headless" OFF)


IF(headless)
   MESSAGE( STATUS "Headless build enabled" )
  ADD_DEFINITIONS(-DHEADLESS)
ENDIF()


IF(APPLE)
    # Apple specific
    MESSAGE( STATUS "System: Apple")
    INCLUDE_DIRECTORIES(/usr/local/include/)
    INCLUDE_DIRECTORIES(/usr/local/include/libfreenect)
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
ENDIF()

IF(WIN32)
    # Windows specific
    MESSAGE( STATUS "System: Windows")

    IF(MINGW)
        MESSAGE( STATUS "Compiler: MinGW")
        SET (CMAKE_CXX_FLAGS "-std=c++11 -Wall")
    ENDIF()

    IF(MSVC)
        MESSAGE( STATUS "Compiler: MSVC")
        STRING( TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
        MESSAGE( STATUS "Build type: "${BUILD_TYPE})
        IF(${BUILD_TYPE} STREQUAL "DEBUG")
            add_definitions(-D_ITERATOR_DEBUG_LEVEL=1) # Remove itterator assert
            MESSAGE( STATUS "ITERATOR_DEBUG_LEVEL=1")
            add_definitions(-D_CRT_SECURE_NO_WARNINGS)
            MESSAGE( STATUS "CRT_SECURE_NO_WARNINGS=1")
        ENDIF()

        MESSAGE( STATUS "ENV_OPENCV_MSVC2012_X64_INSTALL_DIR: " $ENV{OPENCV_MSVC2012_X64_INSTALL_DIR})
        MESSAGE( STATUS "ENV_OPENNI2_INCLUDE64: " $ENV{OPENNI2_INCLUDE64} )
        MESSAGE( STATUS "ENV_OPENNI2_LIB64: " $ENV{OPENNI2_LIB64} )

        SET( OPENCV_HINTS $ENV{OPENCV_MSVC2012_X64_INSTALL_DIR})

        FIND_LIBRARY( OPENNI2_LIBS
            NAMES OpenNI2
            HINTS $ENV{OPENNI2_LIB64}
            PATH_SUFFIXES lib lib64)
        FIND_PATH( OPENNI2_INCLUDE_DIR OpenNI.h
            HINTS $ENV{OPENNI2_INCLUDE64}
            PATH_SUFFIXES openni2 ni2)

        MESSAGE( STATUS "OPENNI2_INCLUDE_DIR: " ${OPENNI2_INCLUDE_DIR} )
        MESSAGE( STATUS "OPENNI2_LIBS: " ${OPENNI2_LIBS} )

        cmake_policy(SET CMP0020 NEW) # Remove cmake warnings
    ENDIF()
ENDIF()

# Tell CMake to run moc when necessary:
IF(NOT headless)
 set(CMAKE_AUTOMOC ON)
ENDIF()
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#move the configurationfile to the build dir
CONFIGURE_FILE(../conf/dense_conf.yml dense_conf.yml COPYONLY)
CONFIGURE_FILE(../conf/guiConfig.yml guiConfig.yml COPYONLY)

# Load external libraries
IF(NOT headless)
	find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
ENDIF()

find_package(OpenCV REQUIRED HINTS ${OPENCV_HINTS})
MESSAGE( STATUS "OPENCV_INCLUDE_DIRS: " ${OpenCV_INCLUDE_DIRS} )
MESSAGE( STATUS "OPENCV_LIB_DIR: " ${OpenCV_LIB_DIR} )

# Include
IF(NOT headless)
  INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
ENDIF()

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENNI2_INCLUDE_DIR})

# Load project libs
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Debugging)
ADD_SUBDIRECTORY(ImageProcessing)
ADD_SUBDIRECTORY(Network)
ADD_SUBDIRECTORY(Analytics)
ADD_SUBDIRECTORY(Utilities)
ADD_SUBDIRECTORY(Evaluation)
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
    Evaluation
    # ExtLib
    ${OpenCV_LIBS}
    ${OPENNI2_LIBS}
)

# Activate Qt for project
# Must be the last line in this file, else { FAIL }
IF(NOT headless)
qt5_use_modules(dense_kitchen Core Gui Widgets)
ENDIF()
