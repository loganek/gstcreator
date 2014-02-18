include_directories(include/Properties)

set(PROPERTIES_HEADERS 
	include/Properties/Property.h
	include/Properties/PropertyString.h
)

add_library(Properties
	Property.cpp
	PropertyString.cpp
	${PROPERTIES_HEADERS}
)

qt5_use_modules(Properties Widgets)