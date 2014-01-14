set(LEX_PARS_INSPECTOR_HEADERS
	include/LexPars/Lexer.h
	include/LexPars/Parser.h
)

add_library(LexPars
	Lexer.cpp
	${PLUGINS_INSPECTOR_HEADERS}
)

include_directories(include/LexPars)
