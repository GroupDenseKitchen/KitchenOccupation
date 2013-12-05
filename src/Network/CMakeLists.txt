#Network module
IF(UNIX)
    # Apple specific
    add_library(Network Network.cpp KinectHandlerFreenect.cpp)
ENDIF()

IF(WIN32)
    # Windows specific
    add_library(Network Network.cpp KinectHandlerOpenNi.cpp)
ENDIF()

