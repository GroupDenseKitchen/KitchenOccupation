#Debug module
#this module is magical.

add_library(Debugging Logger.cpp)

IF(NOT headless)
add_library(Debugging MainDebugWindow.cpp MainDebugWindow.ui DebugViewWidget.cpp DebugViewWidget.ui
	    DebugViewGrid.cpp DebugViewGrid.ui MainConfigurationWindow.cpp MainConfigurationWindow.ui)

qt5_wrap_ui(ui_MainDebugWindow.h MainDebugWindow.ui)
qt5_wrap_ui(ui_DebugViewWidget.h DebugViewWidget.ui)
qt5_wrap_ui(ui_DebugViewGrid.h DebugViewGrid.ui)
qt5_wrap_ui(ui_MainConfigurationWindow.h MainConfigurationWindow.ui)
ENDIF()
