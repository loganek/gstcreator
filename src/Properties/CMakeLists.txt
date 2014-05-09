include_directories(include/Properties)

set(PROPERTIES_HEADERS 
	include/Properties/Property.h
	include/Properties/PropertyString.h
	include/Properties/PropertyBoolean.h
	include/Properties/PropertyEnum.h
	include/Properties/ILockable.h
)

add_library(Properties
	Property.cpp
	PropertyString.cpp
	PropertyBoolean.cpp
	PropertyEnum.cpp
	${PROPERTIES_HEADERS}
)

qt5_use_modules(Properties Widgets)