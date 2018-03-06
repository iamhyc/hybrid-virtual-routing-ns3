/* -*- Mode:C++; c-file-style:"gnu"; -*- */
/*
 * Mark@Lab1112, EEE, SUSTech
 * support following topology scenario;
 * 1. single-layer-wifi
 * 2. vlc-p2p-relay-wifi
 * 3. vlc-p2p-relay-wifi-hybrid
 * 4. vlc-p2m-relay-wifi
 * 5. wifi-p2p-relay-wifi
 */
#include <iostream>

#include "main.h"
#include "ns3/object.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("HybridVirtualRouting");

// {Node, Channel, Device, Stack, Address, Interface, Service, Application}
// nodes
static NodeContainer		collisionNodes;
static NetDeviceContainer	collisionDevices;
// channel helper
static PointToPointHelper pointToPoint;
static CsmaHelper csma;
static WifiHelper wifi;
static WifiMacHelper mac;
static MobilityHelper mobility;
// service helper
static InternetStackHelper stack;
static Ipv4AddressHelper address;

using namespace rapidjson;
void printDocument(char const *name="", Value const *doc=0, int layer=0)
{
	string m_indent(layer, '\t');
	if (name != "")
	{
		printf("%s%s: \n", m_indent.c_str(), name);
	}

	for(auto& m : doc->GetObject())
	{
		auto m_type = m.value.GetType();
		switch(m_type)
		{
			case 3: //JSON Object
				printDocument(m.name.GetString(), &m.value, layer+1);
				break;
			case 4: //Array
				if (m.value[0].IsObject())
				{
					printf("%s\t\"%s\" [\n", m_indent.c_str(), m.name.GetString());
					printDocument("", &m.value[0], layer+1);
					printf("%s\t]\n", m_indent.c_str());
				}
				break;
			default:
				printf("\t%s\"%s\" is %s\n", m_indent.c_str(), m.name.GetString(), kTypeNames[m_type]);
				break;
		}
	}
	return;
}

/*using ns3::NodeContainer
Create(); Add(); Get();

helper.Install(NodeContainer);
//wifi channel, {Area, Numbers, Range, Mobility, Loss, Delay}

stack.install(NodeContainer)
address.setBase("Interface", "NetMask");
address.Assign(NetDeviceContainer);*/

int main(int argc, char *argv[])
{
	/* Global Static Configuration */
	bool verbose;
	
	/* Command Parameter Parse */
	CommandLine cmd;
	cmd.Parse (argc, argv);

	//interconnect with CSMA, equipped with WiFi-AP and P2P
	//interconnect with Wi-Fi Sta, equipped with P2P
	/* 1.Node Container Setup */
	/* 2.Channel Helper Setup */
	/* 3.Internet Stack Install */
	/* 4.Application Install */

	/* 5.Simulator Setup */
	Time::SetResolution(Time::NS);
	if (verbose)
	{
		// pointToPoint.EnablePcapAll("main-p2p");
		// LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
		// LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	}

	/* Run Simulator */
	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}