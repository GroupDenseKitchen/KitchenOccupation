CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)
# Dense kitchen project
PROJECT(dense_kitchen)

# is TRUE on Apple OS X
MESSAGE( STATUS "APPLE: " ${APPLE} )
# is TRUE when using the MinGW compiler in Windows
MESSAGE( STATUS "MINGW: " ${MINGW} )

IF(APPLE)
    # Apple specific
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11 -ox")
ENDIF()

IF(WIN32)
    # Windows specific
    IF(MINGW)
        SET (CMAKE_CXX_FLAGS "-std=c++11 -Wall")
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
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(OpenCV REQUIRED)

# Include
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

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
)

# Activate Qt for project
# Must be the last line in this file, else { FAIL }
qt5_use_modules(dense_kitchen Core Gui Widgets)
