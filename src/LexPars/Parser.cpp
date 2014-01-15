/*
 * gstcreator
 *     Parser.cpp
 *
 *  Created on: 15 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Parser.h"
#include <stdexcept>

using namespace std;

ObjectExpression* Parser::parse(const string& text)
{
	lexer.set_buffer(text);
	lexer.get_next_token(Token::IDENTIFIER);

	return parse_identifier();
}

ObjectExpression* Parser::parse_identifier()
{
	ObjectExpression* current_object = nullptr;
	Token token = lexer.get_next_token(Token::OPERATOR | Token::EOL);

	if (token == Token::EOL)
		return new ObjectExpression(lexer.get_identifier());

	switch (lexer.get_operator())
	{
	case '.':
	{
		current_object = new ObjectExpression(lexer.get_identifier());
		lexer.get_next_token(Token::IDENTIFIER);
		current_object->set_child(parse_identifier());
		break;
	}
	case ')':
		current_object = new ObjectExpression(lexer.get_identifier());
		break;
	case '(':
	{
		string name = lexer.get_identifier();
		auto v =  parse_arguments();
		current_object = new MethodExpression(name, v);
		lexer.get_next_token(Token::OPERATOR | Token::EOL);

		if (token == Token::OPERATOR && lexer.get_operator() == '.')
		{
			lexer.get_next_token(Token::IDENTIFIER);
			current_object->set_child(parse_identifier());
		}
		break;
	}
	default:
		break;
	}

	return current_object == nullptr ? new ObjectExpression(lexer.get_identifier()) : current_object;
}

StringExpression* Parser::parse_string()
{
	return new StringExpression(lexer.read_until('"'));
}

std::vector<ObjectExpression*> Parser::parse_arguments()
{
	std::vector<ObjectExpression*> args;

	while (true)
	{
		Token token = lexer.get_next_token(Token::OPERATOR | Token::IDENTIFIER);

		if (token == Token::OPERATOR)
		{
			if (lexer.get_operator() == ')')
				return args;
			if (lexer.get_operator() == '"')
			{
				args.push_back(new StringExpression(lexer.read_until('"')));
				lexer.get_next_token(Token::OPERATOR);
			}
		}
		else if (token == Token::IDENTIFIER)
		{
			args.push_back(parse_identifier());
			if (lexer.get_operator() == ')')
				return args;
		}
	}

	return args;
}

