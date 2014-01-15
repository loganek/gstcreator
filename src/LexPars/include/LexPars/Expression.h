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

class MethodExpression;

class ObjectExpression
{
private:
	ObjectExpression* child;

protected:
	std::string name;
	MethodExpression* method;

public:
	virtual ~ObjectExpression();
	ObjectExpression(const std::string& name);

	std::string get_name() const;
	ObjectExpression* get_child() const;
	MethodExpression* get_method() const;
	void set_child(ObjectExpression* child);
	void set_method(MethodExpression* method);
};

class MethodExpression : public ObjectExpression
{
private:
	std::vector<ObjectExpression*> args;

public:
	MethodExpression(const std::string& name, std::vector<ObjectExpression*>& args);
	virtual ~MethodExpression();
	std::vector<ObjectExpression*> get_args() const;
};

class StringExpression : public ObjectExpression
{
public:
	StringExpression(const std::string& name);
};

#endif /* EXPRESSION_H_ */
