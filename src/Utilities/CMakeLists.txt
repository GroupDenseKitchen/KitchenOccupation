#Utilities library

IF(headless)
add_library(Utilities FrameList.cpp Frame.cpp CameraObject.cpp masking.cpp
			Object.cpp Algorithm.cpp AlgorithmFactory.cpp StringUtilities.cpp Timer.cpp)
ELSE()
add_library(Utilities FrameList.cpp Frame.cpp CameraObject.cpp 
			Object.cpp Algorithm.cpp AlgorithmFactory.cpp StringUtilities.cpp Timer.cpp)
ENDIF()