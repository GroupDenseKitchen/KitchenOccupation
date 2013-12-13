#Debug module
#this module is magical.

IF(headless)
add_library(Debugging Logger.cpp)
add_library(Debugging Logger.cpp MainDebugWindow.cpp MainDebugWindow.ui DebugViewWidget.cpp DebugViewWidget.ui
            DebugViewGrid.cpp DebugViewGrid.ui MainConfigurationWindow.cpp MainConfigurationWindow.ui
            CalibrationWindow.cpp CalibrationWindow.ui)
ELSE()
qt5_wrap_ui(ui_MainDebugWindow.h MainDebugWindow.ui)
qt5_wrap_ui(ui_DebugViewWidget.h DebugViewWidget.ui)
qt5_wrap_ui(ui_DebugViewGrid.h DebugViewGrid.ui)
qt5_wrap_ui(ui_MainConfigurationWindow.h MainConfigurationWindow.ui)

add_library(Debugging Logger.cpp MainDebugWindow.cpp MainDebugWindow.ui DebugViewWidget.cpp DebugViewWidget.ui
           DebugViewGrid.cpp DebugViewGrid.ui MainConfigurationWindow.cpp MainConfigurationWindow.ui)
ENDIF()
