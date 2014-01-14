/*
 * gstcreator
 *     Lexer.cpp
 *
 *  Created on: 14 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "LexPars/Lexer.h"
#include <gtest/gtest.h>

TEST(Lexer, CheckReadingToken)
{
	Lexer lexer;
	lexer.set_buffer("marcin.kolny");

	ASSERT_EQ(Token::IDENTIFIER, lexer.get_next_token());
	ASSERT_EQ("marcin", lexer.get_identifier());
	ASSERT_EQ(Token::OPERATOR, lexer.get_next_token());
	ASSERT_EQ('.', lexer.get_operator());
	ASSERT_EQ(Token::IDENTIFIER, lexer.get_next_token());
	ASSERT_EQ("kolny", lexer.get_identifier());
}
