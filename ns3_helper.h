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
	PointToPointHelper pointToPoint;
	CsmaHelper csma;
	WifiHelper wifi;
	WifiMacHelper mac;
	MobilityHelper mobility;
	// service helper
	InternetStackHelper stack;
	Ipv4AddressHelper address;
}

#endif