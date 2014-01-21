/*
 * gstcreator
 *     CommandFactory.h
 *
 *  Created on: 16 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include "LexPars/Expression.h"
#include "Command.h"
#include <gstreamermm.h>
#include <memory>

class CommandFactory
{
private:
	Glib::RefPtr<Gst::Element> model;
	Glib::RefPtr<Gst::Object> gst_object;
	ObjectExpression* object;
	MethodExpression* method;
	std::shared_ptr<Command> process_method();

public:
	CommandFactory(ObjectExpression* object, const Glib::RefPtr<Gst::Element>& model);
	std::shared_ptr<Command> process();

};

#endif /* COMMANDFACTORY_H_ */
