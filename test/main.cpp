/*
 * gstcreator
 *     main.cpp
 *
 *  Created on: 13 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include <gtest/gtest.h>
#include <gstreamermm.h>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	Gst::init(argc, argv);

	return RUN_ALL_TESTS();
}

