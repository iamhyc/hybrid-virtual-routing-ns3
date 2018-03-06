
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

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"

static const char* kTypeNames[] = 
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };
void printDocument(char const *name="", Value const *doc=0, int layer=0);

namespace ns_net
{
	const char* p2pAddressMask[8] = { 127, 191, 223, 239, 247, 251, 253, 254 };
	const char* kChannelNames[] = { "csma", "wifi", "p2p" };

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
}

#endif
