/*
 * gstcreator
 *     Parser.h
 *
 *  Created on: 14 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Lexer.h"
#include "Expression.h"

class Parser
{
private:
	Lexer lexer;

	ObjectExpression* parse_identifier();
	StringExpression* parse_string();
	std::vector<ObjectExpression*> parse_arguments();

public:
	ObjectExpression* parse(const std::string& text);
};

#endif /* PARSER_H_ */
