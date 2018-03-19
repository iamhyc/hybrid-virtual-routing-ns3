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

	// channel helper
	extern PointToPointHelper p2p;
	extern CsmaHelper csma;
	/*wifi channel: {Area, Numbers, Range, Mobility, Loss, Delay}*/
	extern WiFiManager wifi_manager;
	extern WifiHelper wifi;
	extern WifiMacHelper wifi_mac;
	extern MobilityHelper mobility;
	// service helper
	extern InternetStackHelper stack;
	extern Ipv4AddressHelper address;
}

#endif