#ifndef __NS3_HELPER_H__
#define __NS3_HELPER_H__

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

namespace ns3_helper
{
	using namespace ns3;
	typedef std::map<std::string, WifiHelper> WiFiManager;
	typedef NodeContainer			Nodes;
	typedef NetDeviceContainer		Nets;
	typedef Ipv4InterfaceContainer	Ifaces;

	typedef struct generalSchema
	{
		float throughput;
		float delay;
	}generalSchema_t;

	typedef struct wifiSchema
	{
		char* ssid;
		char* standard;//IEEE 802.11
		int mobility[4];
		int channel;
		int bandwidth;
	}wifiSchema_t;

	void p2pDefaultHelper(Nodes const &, Nets &, Ifaces &);
	void csmaDefaultHelper(Nodes const &, Nets &, Ifaces &);
	void wifiDefaultHelper(Nodes const &, Nets &, Ifaces &);
}

#endif
