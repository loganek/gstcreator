qt5_wrap_ui(UIS_HDRS
	ui_files/MainWindow.ui
)

set(GUI_HEADERS 
	include/Gui/MainWindow.h
)

add_library(Gui
	MainWindow.cpp
	${GUI_HEADERS}
	${UIS_HDRS})

include_directories(include/Gui)

qt5_use_modules(Gui Widgets)