/*
 * gstcreator
 *     Expression.h
 *
 *  Created on: 15 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <list>
#include <string>
#include <vector>

enum class ExpressionType
{
	OBJECT = 0,
	METHOD = 1,
	STRING = 2
};

class MethodExpression;

class ObjectExpression
{
private:
	ObjectExpression* child;

protected:
	std::string name;

public:
	virtual ~ObjectExpression();
	ObjectExpression(const std::string& name);

	std::string get_name() const;
	ObjectExpression* get_child() const;
	MethodExpression* as_method();
	void set_child(ObjectExpression* child);
	virtual ExpressionType get_type() const
	{ return ExpressionType::OBJECT; }
	bool is_method() const;
};

class MethodExpression : public ObjectExpression
{
private:
	std::vector<ObjectExpression*> args;

public:
	MethodExpression(const std::string& name, std::vector<ObjectExpression*>& args);
	virtual ~MethodExpression();
	std::vector<ObjectExpression*> get_args() const;
	ExpressionType get_type() const
	{ return ExpressionType::METHOD; }
};

class StringExpression : public ObjectExpression
{
public:
	StringExpression(const std::string& name);
	ExpressionType get_type() const
	{ return ExpressionType::STRING; }
};

#endif /* EXPRESSION_H_ */
