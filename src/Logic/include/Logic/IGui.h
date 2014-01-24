/*
 * gstcreator
 *     IGui.h
 *
 *  Created on: 23 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef IGUI_H_
#define IGUI_H_

#include <memory>
#include "Logic/MainController.h"

class IGui
{
public:
	virtual ~IGui(){}

	virtual void set_controller(std::shared_ptr<MainController> controller) = 0;
	virtual void current_model_changed(const std::string& model_path) = 0;
};

#endif /* IGUI_H_ */
