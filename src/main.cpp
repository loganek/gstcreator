/*
 * gstcreator
 *     main.cpp
 *
 *  Created on: 12 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Gui/MainWindow.h"
#include "Logic/MainController.h"
#include <QApplication>
#include <gstreamermm.h>

int main(int argc, char** argv)
{
	Gst::init(argc, argv);
	QApplication a(argc, argv);
	MainWindow w;
	std::shared_ptr<MainController> controller(new MainController());
	w.set_controller(controller);
	w.show();

	return a.exec();
}

