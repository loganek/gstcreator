/*
 * gstcreator
 *     MainController.h
 *
 *  Created on: 22 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#include "GstController.h"
#include "LexPars/Parser.h"

class IGui;

class MainController
{
private:
	GstController gst_controller;
	Parser parser;
	IGui* gui;

	template<typename T, typename Ret, typename ...Args>
	static void safe_call(T* object, void (T::* fun)(Args...), Args... args);

public:
	MainController();
	virtual ~MainController() {}

	void update_current_model(const std::string& model_path);

	void call_command(const std::string& cmd_text);
	void set_gui(IGui* gui);
};

#endif /* MAINCONTROLLER_H_ */
