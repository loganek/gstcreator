set(UTILS_HEADERS
	include/Utils/GstUtils.h 
	include/Utils/StringUtils.h
)

add_library(Utils
	GstUtils.cpp
	StringUtils.cpp
	${UTILS_HEADERS}
)

include_directories(include/Utils)
