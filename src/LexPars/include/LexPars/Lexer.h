/*
 * gstcreator
 *     Lexer.h
 *
 *  Created on: 14 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <string>

enum class Token
{
	INVALID,
	IDENTIFIER,
	ACCESS_OBJECT,
	ACCESS_STATIC,
	VAR_DEF,
};

class Lexer
{
	char last_char;
	std::string current_identifier;
	Token current_token;

	Token get_next_token();
	Token get_token();
};


#endif /* LEXER_H_ */
