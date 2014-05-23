/*
 * GstProbeManager.h
 *
 *  Created on: May 23, 2014
 *      Author: loganek
 */

#ifndef GSTPROBEMANAGER_H_
#define GSTPROBEMANAGER_H_

#include <gstreamermm.h>
#include <utility>
#include <vector>

// todo move to another file this hack
#include <type_traits>
#include <sigc++/sigc++.h>
namespace sigc
{
template <typename Functor>
struct functor_trait<Functor, false>
{
	typedef decltype (::sigc::mem_fun (std::declval<Functor&> (),
			&Functor::operator())) _intermediate;

	typedef typename _intermediate::result_type result_type;
	typedef Functor functor_type;
};
}

class GstProbeManager
{
private:
	std::map<Glib::RefPtr<Gst::Pad>, std::map<Gst::PadProbeType, gulong>> probe_ides;

public:
	virtual ~GstProbeManager() {}

	static std::vector<std::pair<std::string,Gst::PadProbeType>> get_available_probes ();

	void set_probe (Glib::RefPtr<Gst::Pad> pad, Gst::PadProbeType type);
	void remove_probe (Glib::RefPtr<Gst::Pad> pad, Gst::PadProbeType type);
	bool is_probe_exists (Glib::RefPtr<Gst::Pad> pad, Gst::PadProbeType type);
};

#endif /* GSTPROBEMANAGER_H_ */
