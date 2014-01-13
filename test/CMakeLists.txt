find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS}
		${GCS_SOURCE_DIR}/src)
	
	
set(SOURCE main.cpp)
	
add_subdirectory(Commands)

add_executable(Test ${SOURCE})
target_link_libraries(Test Commands ${GSTMM_LIBRARIES} ${GTEST_BOTH_LIBRARIES})

