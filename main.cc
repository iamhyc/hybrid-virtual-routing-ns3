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

void nodesInit()
{
	// //init P2P nodes
	// p2pNodes.Create(2);
	// // init CSMA nodes
	// csmaNodes.Add (p2pNodes.Get(1));
	// csmaNodes.Create (3);
	// // init wifi nodes
	// wifiApNode = p2pNodes.Get (0);
	// wifiStaNodes.Create(3);
}

void topologyInit()
{
	// //p2p channel
	// pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	// pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
	// /* 1+2 Channel->Node */
	// p2pDevices = pointToPoint.Install (p2pNodes);
	// //wifi channel, {Area, Numbers, Range, Mobility, Loss, Delay}
}

void networkInit()
{
	// // for p2p device
	// stack.Install (p2pNodes);
	// address.SetBase ("192.168.3.0", "255.255.255.0");
	// p2pInterfaces = address.Assign (p2pDevices);
	// // for csma device
	// stack.Install (csmaNodes);
	// // for wifi device
	// stack.Install (wifiApNode);
	// stack.Install (wifiStaNodes);
}

int main(int argc, char *argv[])
{
	/* Global Static Configuration */
	//interconnect with CSMA, equipped with WiFi-AP and P2P
	static RouterClass routerNodes;
	//interconnect with Wi-Fi Sta, equipped with P2P
	static RelayGroup relayGroup(routerNodes);
	
	/* Command Parameter Parse */
	CommandLine cmd;
	cmd.Parse (argc, argv);

	/* 1.Node Container Setup */
	nodesInit();
	/* 2.Channel Helper Setup */
	topologyInit();
	/* 3.Internet Stack Install */
	networkInit();

	/* 4.Application Install */
	/*UdpEchoServerHelper echoServer (9);

	ApplicationContainer serverApps = echoServer.Install (p2pNodes.Get (1));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient (p2pInterfaces.GetAddress (1), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

	ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (10.0));*/

	/* 5.Simulator Setup */
	Time::SetResolution (Time::NS);
	if (verbose)
	{
		pointToPoint.EnablePcapAll("main-p2p");
		LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
		LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	}

	/* Run Simulator */
	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}