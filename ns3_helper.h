#ifndef __NS3_HELPER_H__
#define __NS3_HELPER_H__

#define UNUSED(x) (void)(x)
#define R_T(x)	"\033[31m" x "\033[0m"
#define G_T(x)	"\033[32m" x "\033[0m"
#define Y_T(x)	"\033[33m" x "\033[0m"
#define B_T(x)	"\033[34m" x "\033[0m"
#define M_T(x)	"\033[35m" x "\033[0m"
#define C_T(x)	"\033[36m" x "\033[0m"

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

	enum NS3Link {P2P, CSMA, WIFI};

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
	void wifiDefaultHelper(Ptr<Node> const &, Nodes const &, Nets &, Ifaces &);
}

#endif
