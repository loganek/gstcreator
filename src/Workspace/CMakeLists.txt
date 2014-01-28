set(WORKSPACE_HEADERS 
	include/Workspace/WorkspaceWidget.h
	include/Workspace/EventFilter.h
)

add_library(Workspace
	EventFilter.cpp
	WorkspaceWidget.cpp
	${WORKSPACE_HEADERS}
)

include_directories(include/Workspace)

qt5_use_modules(Workspace Widgets)
