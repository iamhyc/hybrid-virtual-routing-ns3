#ifndef __NS3_HELPER_H__
#define __NS3_HELPER_H__

#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"

namespace ns3_helper
{
	// channel helper
	static PointToPointHelper p2p;
	static CsmaHelper csma;
	/*wifi channel: {Area, Numbers, Range, Mobility, Loss, Delay}*/
	static WifiHelper wifi;
	static WifiMacHelper wifi_mac;
	static MobilityHelper mobility;
	// service helper
	static InternetStackHelper stack;
	static Ipv4AddressHelper address;
}

#endif