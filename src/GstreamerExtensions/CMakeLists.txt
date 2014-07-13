set(GSTREAMER_EXTENSIONS_HEADERS 
	include/GstreamerExtensions/Link.h
	include/GstreamerExtensions/SometimesPad.h
)

add_library(GstreamerExtensions
	Link.cpp
	SometimesPad.cpp
	${GSTREAMER_EXTENSIONS})
	
include_directories(include/GstreamerExtensions)
