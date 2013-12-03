#Network module
IF(APPLE)
    # Apple specific
    add_library(Network Network.cpp KinectHandlerFreenect.cpp)
ENDIF()

IF(WIN32)
    # Windows specific
    IF(MINGW)
        add_library(Network Network.cpp KinectHandlerOpenNi.cpp)
    ENDIF()
ENDIF()

