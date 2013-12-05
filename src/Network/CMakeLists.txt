#Network module
IF(UNIX)
    # Apple specific
    ADD_LIBRARY(Network Network.cpp KinectHandlerFreenect.cpp)
    TARGET_LINK_LIBRARIES(Network freenect freenect_sync)

ENDIF()

IF(WIN32)
    # Windows specific
    add_library(Network Network.cpp KinectHandlerOpenNi.cpp)
ENDIF()

