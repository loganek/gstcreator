/*
 * gstcreator
 *     Expression.cpp
 *
 *  Created on: 15 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Expression.h"
#include <stdexcept>

ObjectExpression::ObjectExpression(const std::string& name)
: child(nullptr),
  name(name)
{
}

ObjectExpression::~ObjectExpression()
{
	if (child != nullptr)
		delete child;
}

void ObjectExpression::set_child(ObjectExpression* child)
{
	this->child = child;
}

std::string ObjectExpression::get_name() const
{
	return name;
}

ObjectExpression*  ObjectExpression::get_child() const
{
	return child;
}

MethodExpression*  ObjectExpression::as_method()
{
	if (!is_method())
		throw std::runtime_error("object is not a method");

	return static_cast<MethodExpression*>(this);
}

bool ObjectExpression::is_method() const
{
	return get_type() == ExpressionType::METHOD;
}

MethodExpression::MethodExpression(const std::string& name, std::vector<ObjectExpression*>& args)
: ObjectExpression(name), args(args)
{}

MethodExpression::~MethodExpression()
{
	while (args.size())
	{
		delete args[0];
		args.erase(args.begin());
	}
}

std::vector<ObjectExpression*> MethodExpression::get_args() const
{
	return args;
}

StringExpression::StringExpression(const std::string& name)
: ObjectExpression(name)
{}
