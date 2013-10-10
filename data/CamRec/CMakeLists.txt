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


