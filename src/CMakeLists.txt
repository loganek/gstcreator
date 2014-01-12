add_subdirectory(Gui)

include_directories(
	${PROJECT_SOURCE_DIR}/Gui/include)

add_executable(gstcreator main.cpp)

target_link_libraries(gstcreator Gui)

qt5_use_modules(gstcreator Widgets)