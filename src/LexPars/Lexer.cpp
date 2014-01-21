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
vector<char> Lexer::operators = { '.', '(', ')', ',', '"' };

Token operator|(Token l, Token r)
{
	return static_cast<Token>(static_cast<int>(l)|static_cast<int>(r));
}

Token operator&(Token l, Token r)
{
	return static_cast<Token>(static_cast<int>(l)&static_cast<int>(r));
}

Lexer::Lexer()
{
	reset();
}

bool Lexer::is_correct_sign_name(char c)
{
	return isalnum(c) || c == '_' || c == '-';
}

Token Lexer::get_token()
{
	while (isspace(last_char))
		last_char = read_char();

	if (last_char == -1)
		return Token::EOL;

	if (isalpha(last_char))
	{
		current_identifier = last_char;
		while (is_correct_sign_name((last_char = read_char())))
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

	if (last_char == -1)
		return Token::EOL;

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

Token Lexer::get_next_token(Token expected)
{
	current_token = get_token();

	if ((current_token & expected) == Token::INVALID)
		// TODO token to human readable text conversion
		throw std::runtime_error("Expected " + std::to_string((int)expected) + " token, but " + std::to_string((int)current_token) + " occurred");

	return current_token;
}

std::string Lexer::read_until(int sign)
{
	std::string raw;

	while (last_char != sign)
	{
		raw += last_char;
		last_char = read_char();
	}

	return raw;
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
