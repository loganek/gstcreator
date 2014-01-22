/*
 * gstcreator
 *     MainController.cpp
 *
 *  Created on: 22 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "MainController.h"

MainController::MainController()
: gst_controller(Gst::Pipeline::create())
{
}
