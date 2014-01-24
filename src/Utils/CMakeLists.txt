set(UTILS_HEADERS
	include/Utils/GstUtils.h 
	include/Utils/StringUtils.h
	include/Utils/EnumUtils.h
)

add_library(Utils
	GstUtils.cpp
	StringUtils.cpp
	EnumUtils.cpp
	${UTILS_HEADERS}
)

include_directories(include/Utils)
