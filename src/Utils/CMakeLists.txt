set(UTILS_HEADERS 
	include/Utils/StringUtils.h
)

add_library(Utils
	StringUtils.cpp
	${UTILS_HEADERS}
)

include_directories(include/Utils)
