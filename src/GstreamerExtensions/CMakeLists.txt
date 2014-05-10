set(GSTREAMER_EXTENSIONS_HEADERS 
	include/GstreamerExtensions/Link.h
)

add_library(GstreamerExtensions
	Link.cpp
	${GSTREAMER_EXTENSIONS})
	
include_directories(include/GstreamerExtensions)
