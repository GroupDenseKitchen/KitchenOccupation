#Network module
IF(UNIX)
    # Apple specific
    MESSAGE("Building unix version of network, using freenect")
    ADD_LIBRARY(Network Network.cpp KinectHandlerFreenect.cpp)
    TARGET_LINK_LIBRARIES(Network freenect freenect_sync)
ENDIF()

IF(WIN32)
    # Windows specific
    MESSAGE("Windows detected, building crippled version")
    add_library(Network Network.cpp KinectHandlerOpenNi.cpp)
ENDIF()

