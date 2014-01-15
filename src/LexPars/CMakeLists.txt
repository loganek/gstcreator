set(LEX_PARS_INSPECTOR_HEADERS
	include/LexPars/Expression.h
	include/LexPars/Lexer.h
	include/LexPars/Parser.h
)

add_library(LexPars
	Expression.cpp
	Lexer.cpp
	Parser.cpp
	${PLUGINS_INSPECTOR_HEADERS}
)

include_directories(include/LexPars)
