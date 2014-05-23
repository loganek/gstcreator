set(LOGIC_HEADERS
	include/Logic/GstController.h
	include/Logic/MainController.h
	include/Logic/IModelObserver.h
	include/Logic/GstProbeManager.h
	include/Logic/IProbeObserver.h
)

include_directories(include/Logic)

add_library(Logic
	GstController.cpp
	MainController.cpp
	GstProbeManager.cpp
	${LOGIC_HEADERS}
)

target_link_libraries(Logic LexPars Commands)

