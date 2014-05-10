include_directories(include/qnelibrary)

set(QNELIBRARY_HEADERS 
	include/qnelibrary/qneblock.h
	include/qnelibrary/qneconnection.h
	include/qnelibrary/qneport.h
)

add_library(qnelibrary
	qneblock.cpp
	qneconnection.cpp
	qneport.cpp
	${QNELIBRARY_HEADERS}
)

target_link_libraries(qnelibrary GstreamerExtensions)

qt5_use_modules(qnelibrary Widgets)