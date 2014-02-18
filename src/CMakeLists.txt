set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Utils)

execute_process(
	COMMAND perl
	${UTILS_DIR}/enum_string_generator.pl
	${UTILS_DIR}/../Commands/include/Commands/Command.h
	${UTILS_DIR}/include/Utils/EnumUtils.h
	${UTILS_DIR}/EnumUtils.cpp
	 
)

add_subdirectory(Commands)
add_subdirectory(Gui)
add_subdirectory(LexPars)
add_subdirectory(PluginsInspector)
add_subdirectory(Utils)
add_subdirectory(Logic)
add_subdirectory(qnelibrary)
add_subdirectory(Workspace)
add_subdirectory(Properties)

add_executable(gstcreator main.cpp)

target_link_libraries(gstcreator
	Logic
	Gui)

qt5_use_modules(gstcreator Widgets)