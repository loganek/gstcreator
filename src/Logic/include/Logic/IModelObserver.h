/*
 * IModelObserver.h
 *
 *  Created on: Jan 28, 2014
 *      Author: m.kolny
 */

#ifndef IMODELOBSERVER_H_
#define IMODELOBSERVER_H_

#include <gstreamermm.h>

class IModelObserver
{
public:
	virtual ~IModelObserver(){}
	virtual void pad_added(const Glib::RefPtr<Gst::Pad>& pad) = 0;
	virtual void pad_removed(const Glib::RefPtr<Gst::Pad>& pad) = 0;
	virtual void pad_linked(const Glib::RefPtr<Gst::Pad>& proxy_pad) = 0;
	virtual void pad_unlinked(const Glib::RefPtr<Gst::Pad>& proxy_pad) = 0;
	virtual void element_added(const Glib::RefPtr<Gst::Element>& element) = 0;
	virtual void element_removed(const Glib::RefPtr<Gst::Element>& element) = 0;

};

#endif /* IMODELOBSERVER_H_ */
