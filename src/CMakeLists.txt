add_subdirectory(Commands)
add_subdirectory(Gui)
add_subdirectory(LexPars)
add_subdirectory(PluginsInspector)
add_subdirectory(Utils)
add_subdirectory(Logic)

add_executable(gstcreator main.cpp)

target_link_libraries(gstcreator
	Logic
	Gui)

qt5_use_modules(gstcreator Widgets)