cmake_minimum_required(VERSION 2.8.9)
project(gstcreator)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
SET(PROJECT_SOURCE_DIR "${gstcreator_SOURCE_DIR}/src")

INCLUDE(FindPkgConfig)

add_subdirectory(src)
