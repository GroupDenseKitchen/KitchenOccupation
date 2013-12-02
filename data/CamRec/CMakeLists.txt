CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)
# Cam Recorder
PROJECT(CamRecorder)

IF(APPLE)
    # Apple specific
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
ENDIF()

IF(WIN32)
    # Windows specific
    IF(MINGW)
        # MingW specific
    ENDIF()
ENDIF()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Load external libraries
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(OpenCV REQUIRED)

#SET(OPEN_NI_INCLUDE "C:\Program Files (x86)\OpenNI2\Include")
#FIND_PATH(OPEN_NI_INCLUDE "XnOpenNI.h" "OpenNIConfig.h" HINTS "$ENV{OPEN_NI_INCLUDE}")
#FIND_LIBRARY(OPEN_NI_LIBRARY NAMES OpenNI libOpenNI HINTS $ENV{OPEN_NI_LIB})
#LINK_DIRECTORIES($ENV{OPEN_NI_LIB})
#INCLUDE_DIRECTORIES(${OPEN_NI_INCLUDE})
#LINK_LIBRARIES(${OPEN_NI_LIBRARY})



# Include
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

qt5_wrap_ui(ui_CamRecWindow.h CamRecWindow.ui)

# Binaries
#ADD_EXECUTABLE(CamRec camRec.cpp CamRecWindow.cpp ui_CamRecWindow.h)
ADD_EXECUTABLE(CamRec camRec.cpp CamRecWindow.cpp ui_CamRecWindow.h)

qt5_use_modules(CamRec Widgets)

target_link_libraries(CamRec
    ${OpenCV_LIBS})


