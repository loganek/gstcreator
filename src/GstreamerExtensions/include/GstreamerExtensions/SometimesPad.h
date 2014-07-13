/*
 * SometimesPad.h
 *
 *  Created on: Jul 13, 2014
 *      Author: loganek
 */

#ifndef SOMETIMESPAD_H_
#define SOMETIMESPAD_H_

#include <gstreamermm/pad.h>

class SometimesPad : public Gst::Pad
{
public:
	static Glib::RefPtr<Gst::Pad> create(const Glib::RefPtr<const Gst::PadTemplate>& pad_template, const Glib::RefPtr<Gst::Object>& parent);
};

#endif /* SOMETIMESPAD_H_ */
