/*
 * gstcreator
 *     Lexer.cpp
 *
 *  Created on: 14 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Lexer.h"

Token Lexer::get_token()
{
	do
	{
		last_char = getchar();
	} while (isspace(last_char));

	if (isalpha(last_char))
	{
		current_identifier = last_char;
		while (isalnum((last_char = getchar())))
			current_identifier += last_char;

		if (current_identifier == "var")
			return Token::VAR_DEF;

		return Token::IDENTIFIER;
	}

	if (last_char == '.')
		return Token::ACCESS_OBJECT;

	if (last_char == ':')
	{
		last_char = getchar();
		if (last_char == ':')
			return Token::ACCESS_STATIC;
	}

	return Token::INVALID;
}

Token Lexer::get_next_token()
{
	return current_token = get_token();
}

