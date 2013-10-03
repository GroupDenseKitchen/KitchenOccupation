CMAKE_MINIMUM_REQUIRED (VERSION 2.8.1)
# Dense kitchen project
PROJECT(dense_kitchen)

# is TRUE on Apple OS X
MESSAGE( STATUS "APPLE: " ${APPLE} )
# is TRUE when using the MinGW compiler in Windows
MESSAGE( STATUS "MINGW: " ${MINGW} )

# Something apple
IF(APPLE)
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
ENDIF()

IF(WIN32)
    IF(MINGW)
        # Windows Specific
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

# Load project libs
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Debugging)
ADD_SUBDIRECTORY(ImageProcessing)
ADD_SUBDIRECTORY(Network)
ADD_SUBDIRECTORY(Analytics)
ADD_SUBDIRECTORY(Utilities)
ADD_SUBDIRECTORY(Forms)

# SET(CamRecorder_FORMS Forms/CamRecorderWindow.ui)
#qt5_wrap_ui(CamRecorder_FORMS_HEADERS ${CamRecorder_FORMS})

# Binaries
ADD_EXECUTABLE(dense_kitchen main.cpp DenseKitchen.cpp)
#ADD_EXECUTABLE(CamRecorder camRecorder.cpp ${CamRecorder_FORMS_HEADERS})

# Activate Qt for project
qt5_use_modules(dense_kitchen Core Gui Widgets)
#qt5_use_modules(CamRecorder Core Gui Widgets)

# Link to libraries
TARGET_LINK_LIBRARIES (dense_kitchen 
    Configuration 
    Debugging 
    ImageProcessing 
    Network 
    Statistics 
    Utilities
    ${OpenCV_LIBS}
)

#TARGET_LINK_LIBRARIES (CamRecorder
#    Utilities
#    ${CamRecorder_FORMS_HEADERS}
#    ${OpenCV_LIBS}
#    ${Qt5Widgets_LIBRARIES})


