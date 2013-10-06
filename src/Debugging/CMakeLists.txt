#Debug module
#this module is magical.

add_library(Debugging Logger.cpp MainDebugWindow.cpp DebugView.cpp DebugView.ui MainDebugWindow.ui)
qt5_wrap_ui(ui_MainDebugWindow.h MainDebugWindow.ui)
qt5_wrap_ui(ui_DebugView.h DebugView.ui)
