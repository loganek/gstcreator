qt5_wrap_ui(UIS_HDRS
	ui_files/MainWindow.ui
	ui_files/ExportToDotDialog.ui
	ui_files/GstObjectManagePanel.ui
	ui_files/ProbesWatcherDialog.ui
)

set(GUI_HEADERS 
	include/Gui/MainWindow.h
	include/Gui/ExportToDotDialog.h
	include/Gui/GstObjectManagePanel.h
	include/Gui/ProbesWatcherDialog.h
)

add_library(Gui
	MainWindow.cpp
	ExportToDotDialog.cpp
	GstObjectManagePanel.cpp
	ProbesWatcherDialog.cpp
	${GUI_HEADERS}
	${UIS_HDRS})
	
include_directories(include/Gui)
target_link_libraries(Gui PluginsInspector Workspace Properties Logic)
qt5_use_modules(Gui Widgets)