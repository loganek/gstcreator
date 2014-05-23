/*
 * IProbeObserver.h
 *
 *  Created on: May 23, 2014
 *      Author: loganek
 */

#ifndef IPROBEOBSERVER_H_
#define IPROBEOBSERVER_H_

#include <gstreamermm.h>

class IProbeObserver
{
public:
	virtual ~IProbeObserver(){}
	virtual void event_probe(const Glib::RefPtr<Gst::Pad>& pad, const Glib::RefPtr<Gst::Event>& event) {}
	virtual void query_probe(const Glib::RefPtr<Gst::Pad>& pad, Glib::RefPtr<Gst::Query> query) {}
	virtual void buffer_probe(const Glib::RefPtr<Gst::Pad>& pad, Glib::RefPtr<Gst::Buffer> buffer) {}

};

#endif /* IPROBEOBSERVER_H_ */
