
#ifndef __GLOBAL_CONF_H__
#define __GLOBAL_CONF_H__

#include "include/rapidjson/document.h"

#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"

uint8_t p2pAddressMask[8] = [127, 191, 223, 239, 247, 251, 253, 254];

class RouterClass
{
public:
	RouterClass();
	~RouterClass();
	NodeContainer routerNodes;
};

class RelayGroup
{
public:
	RelayGroup();
	~RelayGroup();
	int addRelayNode();
	int addStationNode();

private:
	NodeContainer relayNodes;
	NodeContainer staNodes;
};

class gConfig
{
public:
	gConfig();
	~gConfig();

private:
	bool verbose;
	
};

#endif
