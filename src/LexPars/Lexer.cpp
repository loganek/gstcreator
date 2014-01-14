/*
 * gstcreator
 *     Lexer.cpp
 *
 *  Created on: 14 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Lexer.h"
#include <algorithm>
#include <stdexcept>

using namespace std;
vector<char> Lexer::operators = { '.', '=' };

Lexer::Lexer()
{
	reset();
}

Token Lexer::get_token()
{
	while (isspace(last_char))
		last_char = read_char();

	if (isalpha(last_char))
	{
		current_identifier = last_char;
		while (isalnum((last_char = read_char())))
			current_identifier += last_char;

		if (current_identifier == "var")
			return Token::VAR_DEF;

		return Token::IDENTIFIER;
	}

	if (find(operators.begin(), operators.end(), last_char) != operators.end())
	{
		current_operator = last_char;
		last_char = read_char();
		return Token::OPERATOR;
	}

	return Token::INVALID;
}

int Lexer::read_char()
{
	if (buffer_pos == buffer.size())
		return -1;
	return buffer[buffer_pos++];
}

Token Lexer::get_next_token()
{
	return current_token = get_token();
}

std::string Lexer::get_identifier()
{
	return current_identifier;
}

char Lexer::get_operator()
{
	return current_operator;
}

void Lexer::set_buffer(const string& buffer)
{
	this->buffer = buffer;
	reset();
}

void Lexer::reset()
{
	buffer_pos = 0;
	last_char = ' ';
	current_operator = -1;
}
