/*
 * GstProbeManager.cpp
 *
 *  Created on: May 23, 2014
 *      Author: loganek
 */

#include "GstProbeManager.h"
#include "Utils/StringUtils.h"
#include <algorithm>

using namespace std;
using namespace Gst;
using Glib::RefPtr;

vector<pair<string,PadProbeType>> GstProbeManager::get_available_probes ()
{
#define Q(x) #x
#define GENERATE_PROBE_PAIR(probe_name) \
		{ \
	string str = StringUtils::to_lower(Q(probe_name)); \
	replace(str.begin(), str.end(), '_', ' ');\
	vect.push_back(make_pair( \
			str, \
			PAD_PROBE_TYPE_ ## probe_name));\
		}

	static vector<pair<string, PadProbeType>> vect;

	if (vect.size() > 0)
		return vect;

	GENERATE_PROBE_PAIR(INVALID)
	GENERATE_PROBE_PAIR(IDLE)
	GENERATE_PROBE_PAIR(BLOCK)
	GENERATE_PROBE_PAIR(BUFFER)
	GENERATE_PROBE_PAIR(BUFFER_LIST)
	GENERATE_PROBE_PAIR(EVENT_DOWNSTREAM)
	GENERATE_PROBE_PAIR(EVENT_UPSTREAM)
	GENERATE_PROBE_PAIR(EVENT_FLUSH)
	GENERATE_PROBE_PAIR(QUERY_DOWNSTREAM)
	GENERATE_PROBE_PAIR(QUERY_UPSTREAM)
	GENERATE_PROBE_PAIR(PUSH)
	GENERATE_PROBE_PAIR(PULL)
	GENERATE_PROBE_PAIR(BLOCKING)
	GENERATE_PROBE_PAIR(DATA_DOWNSTREAM)
	GENERATE_PROBE_PAIR(DATA_UPSTREAM)
	GENERATE_PROBE_PAIR(DATA_BOTH)
	GENERATE_PROBE_PAIR(BLOCK_DOWNSTREAM)
	GENERATE_PROBE_PAIR(BLOCK_UPSTREAM)
	GENERATE_PROBE_PAIR(EVENT_BOTH)
	GENERATE_PROBE_PAIR(QUERY_BOTH)
	GENERATE_PROBE_PAIR(ALL_BOTH)
	GENERATE_PROBE_PAIR(SCHEDULING);

	return vect;
}

void GstProbeManager::set_probe (RefPtr<Pad> pad, PadProbeType type)
{
	remove_probe(pad, type);

	probe_ides[pad][type] = pad->add_probe(type, [](const RefPtr<Pad>& pad, const PadProbeInfo& info) {
		// todo do something here
		return Gst::PAD_PROBE_OK;
	});
}

void GstProbeManager::remove_probe (RefPtr<Pad> pad, PadProbeType type)
{
	if (is_probe_exists(pad, type))
	{
		pad->remove_probe(probe_ides[pad][type]);
		probe_ides[pad].erase(type);
	}
}

bool GstProbeManager::is_probe_exists (RefPtr<Pad> pad, PadProbeType type)
{
	return probe_ides.find(pad) != probe_ides.end() &&
			probe_ides[pad].find(type) != probe_ides[pad].end();
}
