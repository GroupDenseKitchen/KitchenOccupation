#Debug module
#this module is magical.

add_library(Debugging Logger.cpp MainDebugWindow.cpp DebugView.cpp DebugViewWidget.cpp CvImageWidget.cpp DebugView.ui DebugViewWidget.ui MainDebugWindow.ui)

qt5_wrap_ui(ui_MainDebugWindow.h MainDebugWindow.ui)
qt5_wrap_ui(ui_DebugView.h DebugView.ui)
qt5_wrap_ui(ui_DebugViewWidget.h DebugViewWidget.ui)
