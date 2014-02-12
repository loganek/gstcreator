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
#include "Commands.h"
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

	std::shared_ptr<AddCommand> process_add_element();
	std::shared_ptr<AddCommand> process_add_pad();
	std::shared_ptr<StateCommand> process_state_command();
	std::shared_ptr<LinkCommand> process_link_command();
	std::shared_ptr<RemoveCommand> process_remove_element();
	std::shared_ptr<RemoveCommand> process_remove_pad();
	std::shared_ptr<UnlinkCommand> process_unlink();

	static void assert_argument_count(const std::vector<int>& expected, int actual);

public:
	CommandFactory(ObjectExpression* object, const Glib::RefPtr<Gst::Element>& model);
	std::shared_ptr<Command> process();

};

#endif /* COMMANDFACTORY_H_ */
