/*
 * Link.h
 *
 *  Created on: May 10, 2014
 *      Author: loganek
 */

#ifndef LINK_H_
#define LINK_H_

#include <gstreamermm.h>

// todo fixme ugly hack, Link should derived from Gst::Object...
class Link : public Gst::Bus
{
private:
	Glib::RefPtr<Gst::Object> source;
	Glib::RefPtr<Gst::Object> destination;

public:
	Link(const Glib::RefPtr<Gst::Object>& source, const Glib::RefPtr<Gst::Object>& destination);
	virtual ~Link() {}

	Glib::RefPtr<Gst::Object> get_source();
	Glib::RefPtr<Gst::Object> get_destination();
};

#endif /* LINK_H_ */
