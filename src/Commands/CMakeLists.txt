set(COMMANDS_HEADERS 
	include/Commands/Command.h
	include/Commands/AddCommand.h
)

add_library(Commands
	Command.cpp
	AddCommand.cpp
	${COMMANDS_HEADERS})
	
include_directories(include/Commands)
