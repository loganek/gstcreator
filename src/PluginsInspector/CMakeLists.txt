set(PLUGINS_INSPECTOR_HEADERS
	include/PluginsInspector/PluginsInspectorFilter.h
	include/PluginsInspector/PluginsInspectorItem.h
	include/PluginsInspector/PluginsInspectorModel.h
	include/PluginsInspector/PluginsInspectorTreeView.h
)

add_library(PluginsInspector
	PluginsInspectorFilter.cpp
	PluginsInspectorItem.cpp
	PluginsInspectorModel.cpp
	PluginsInspectorTreeView.cpp
	${PLUGINS_INSPECTOR_HEADERS}
)

target_link_libraries(PluginsInspector Utils ${GSTMM_LIBRARIES})
include_directories(include/PluginsInspector)
qt5_use_modules(PluginsInspector Widgets)
