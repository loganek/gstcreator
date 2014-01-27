cmake_minimum_required(VERSION 2.8.9)

project(gstcreator)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
SET(PROJECT_SOURCE_DIR "${gstcreator_SOURCE_DIR}/src")

INCLUDE(FindPkgConfig)

pkg_check_modules(GSTMM REQUIRED gstreamermm-1.0) 
include_directories(${GSTMM_INCLUDE_DIRS})
link_directories(${GSTMM_LIBRARY_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

INCLUDE(FindPkgConfig)

include_directories(
	${PROJECT_SOURCE_DIR}/Commands/include
	${PROJECT_SOURCE_DIR}/qnelibrary/include
	${PROJECT_SOURCE_DIR}/Gui/include
	${PROJECT_SOURCE_DIR}/LexPars/include
	${PROJECT_SOURCE_DIR}/PluginsInspector/include
	${PROJECT_SOURCE_DIR}/Utils/include
	${PROJECT_SOURCE_DIR}/Logic/include)

add_subdirectory(src)
add_subdirectory(test)