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

class MainController : public Parser
{
private:
	GstController gst_controller;

public:
	MainController();
	virtual ~MainController() {}
};

#endif /* MAINCONTROLLER_H_ */
