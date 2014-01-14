find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS}
		${GCS_SOURCE_DIR}/src)
	
	
set(SOURCE main.cpp)
	
add_subdirectory(Commands)
add_subdirectory(LexPars)
add_subdirectory(Utils)

add_executable(Test ${SOURCE})
target_link_libraries(Test Commands Utils ${GSTMM_LIBRARIES} ${GTEST_BOTH_LIBRARIES} pthread)

