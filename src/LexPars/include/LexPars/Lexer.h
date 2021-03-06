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
#include <vector>
#include "Commands/Command.h" // todo temporary solution, this include should be unnecessary


Token operator|(Token l, Token r);
Token operator&(Token l, Token r);

class Lexer
{
private:
	std::string buffer;
	size_t buffer_pos;
	static std::vector<char> operators;
	char last_char;
	int current_operator;
	std::string current_identifier;
	Token current_token;

	Token get_token();
	int read_char();
	void reset();

	static bool is_correct_sign_name(char c);

public:
	Lexer();
	Token get_next_token();
	Token get_next_token(Token expected);
	std::string get_identifier();
	char get_operator();
	void set_buffer(const std::string& buffer);
	std::string read_until(int sign);
};


#endif /* LEXER_H_ */
