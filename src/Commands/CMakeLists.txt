set(COMMANDS_HEADERS 
	include/Commands/Command.h
	include/Commands/AddCommand.h
	include/Commands/StateCommand.h
	include/Commands/CommandFactory.h
	include/Commands/LinkCommand.h
	include/Commands/RemoveCommand.h
)

add_library(Commands
	Command.cpp
	AddCommand.cpp
	StateCommand.cpp
	CommandFactory.cpp
	LinkCommand.cpp
	RemoveCommand.cpp
	${COMMANDS_HEADERS})
	
target_link_libraries(Commands ${GSTMM_LIBRARIES})
	
include_directories(include/Commands)
