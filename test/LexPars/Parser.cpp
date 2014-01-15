/*
 * gstcreator
 *     Lexer.cpp
 *
 *  Created on: 14 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "LexPars/Parser.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(Parser, SimpleIdentifierParse)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("marcin");
	ASSERT_EQ("marcin", exp->get_name());
}

TEST(Parser, AccessToChildObject)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.test");
	ASSERT_EQ("this", exp->get_name());
	ASSERT_EQ("is", exp->get_child()->get_name());
	ASSERT_EQ("test", exp->get_child()->get_child()->get_name());
}

TEST(Parser, NoArgNoReturnMethod)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.method()");
	ASSERT_EQ("this", exp->get_name());
	ASSERT_EQ("is", exp->get_child()->get_name());
	MethodExpression* method = exp->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
	ASSERT_EQ(0, method->get_args().size());
}

TEST(Parser, SimpleArgNoReturnMethod)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.method(arg)");
	MethodExpression* method = exp->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
	ASSERT_EQ(1, method->get_args().size());
	ASSERT_EQ("arg", method->get_args()[0]->get_name());
}

TEST(Parser, ThreeSimpleArgsNoReturnMethod)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.method(arg1, arg2, arg3)");
	MethodExpression* method = exp->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
	ASSERT_EQ(3, method->get_args().size());
	ASSERT_EQ("arg1", method->get_args()[0]->get_name());
	ASSERT_EQ("arg2", method->get_args()[1]->get_name());
	ASSERT_EQ("arg3", method->get_args()[2]->get_name());
}

TEST(Parser, FunctionArgNoReturnMethod)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.method(obj.fun())");
	MethodExpression* method = exp->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
	ASSERT_EQ(1, method->get_args().size());
	ASSERT_EQ("obj", method->get_args()[0]->get_name());
	ASSERT_EQ("fun", method->get_args()[0]->get_method()->get_name());
	ASSERT_EQ(0, method->get_args()[0]->get_method()->get_args().size());
}

TEST(Parser, FunctionArgWithTwoArgsNoReturnMethod)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.method(obj.fun(test1, test2))");
	MethodExpression* method = exp->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
	ASSERT_EQ(1, method->get_args().size());
	ASSERT_EQ("fun", method->get_args()[0]->get_method()->get_name());
	ASSERT_EQ(2, method->get_args()[0]->get_method()->get_args().size());
	ASSERT_EQ("test1", method->get_args()[0]->get_method()->get_args()[0]->get_name());
	ASSERT_EQ("test2", method->get_args()[0]->get_method()->get_args()[1]->get_name());
}

TEST(Parser, ParseStringArguments)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.method(\"str\", ing, \"arg\")");

	MethodExpression* method = exp->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
	ASSERT_EQ(3, method->get_args().size());
	ASSERT_EQ("str", method->get_args()[0]->get_name());
	ASSERT_EQ("ing", method->get_args()[1]->get_name());
	ASSERT_EQ("arg", method->get_args()[2]->get_name());

}

TEST(Parser, FunctionArgWithTwoFunctionArgsWithArgNoReturnMethod)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.is.method(obj.fun(myobj.fun2(), myobj.fun3(test1, test2), test3), \"test4\")");

	MethodExpression* method = exp->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
	ASSERT_EQ(2, method->get_args().size());
	ASSERT_EQ("fun", method->get_args()[0]->get_method()->get_name());
	ASSERT_EQ(3, method->get_args()[0]->get_method()->get_args().size());
	ASSERT_EQ("test4", method->get_args()[1]->get_name());
}

TEST(Parser, FunctionWithReturnedValue)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.method().fun()");
	MethodExpression* method = exp->get_method();
	ASSERT_EQ("fun", method->get_method()->get_name());
}

TEST(Parser, AdvancedFunctionWithReturnedValue)
{
	Parser parser;
	ObjectExpression* exp = parser.parse("this.method(and, \"some\", args, and.function(\"with\", \"args\").with.method(), cool).fun()");
	MethodExpression* method = exp->get_method()->get_args()[3]->get_method()->get_child()->get_method();
	ASSERT_EQ("method", method->get_name());
}
