
#include "ns3_helper.h"
#include <arpa/inet.h>

using namespace ns3_helper;

// channel helper
static PointToPointHelper p2pHelper;
static CsmaHelper csmaHelper;
/*wifi channel: {Area, Numbers, Range, Mobility, Loss, Delay}*/
static WifiHelper wifiHelper;
static YansWifiPhyHelper	wifi_phy;
static WifiMacHelper		wifi_mac;
static MobilityHelper		mobility;
static WiFiManager wifi_manager;
// service helper
static uint32_t addr_counter = 0;
static uint32_t addr_base = inet_addr("10.0.0.0");
static InternetStackHelper stack;
static Ipv4AddressHelper address;

void ns3_helper::InstallStackHelper(Nodes& nodes)
{
	stack.Install(nodes);
}

void IPHelper(char const *base)
{
	char IPAddr[20];
	struct in_addr s;
	strncpy(IPAddr, base, strlen(base));
	inet_pton(AF_INET, IPAddr, &s);
	inet_ntop(AF_INET, (void *)&s, IPAddr, sizeof(in_addr));
}

void ns3_helper::p2pBuilder(KeyPair keyword, flowSchema schema, NodesTuple& parent, NodesTuple& child)
{
	Nets p2pNets;
	Ifaces p2pIfaces;

	p2pHelper.SetDeviceAttribute("DataRate", StringValue(schema.throughput));
	p2pHelper.SetChannelAttribute("Delay", StringValue(schema.delay));
	for(auto it=child.nodes.Begin();it!=child.nodes.End();++it)
	{
		// p2pNets = p2pHelper.Install(p2pNodes);
		// address.SetBase ("10.1.1.0", "255.255.255.0");
		// p2pIfaces = address.Assign (p2pNets);
	}
}

void ns3_helper::csmaBuilder(KeyPair keyword, flowSchema schema, NodesTuple& parent, NodesTuple& child)
{
	csmaHelper.SetChannelAttribute("DataRate", StringValue(schema.throughput));
	csmaHelper.SetChannelAttribute("Delay", StringValue(schema.delay));
	// csmaDevices = csmaHelper.Install(csmaNodes);
}

void ns3_helper::wifiBuilder(KeyPair keyword, wifiSchema schema, NodesTuple& parent, NodesTuple& child)
{
	UNUSED(keyword);
	UNUSED(schema);
	UNUSED(parent);
	UNUSED(child);
}

void wifiDefaultHelper(Ptr<Node> const &wifiApNode, Nodes const &wifiStaNodes,
						Nets &wifiDevices, Ifaces &wifiIfaces)
{
	NetDeviceContainer apDevice, staDevices;

	/* Yans Physical Layer */
	wifi_phy = YansWifiPhyHelper::Default();
	wifi_phy.SetChannel( YansWifiChannelHelper::Default().Create() );

	/* WiFi MAC Layer */
	Ssid ssid = Ssid( "ns3-" + std::to_string( std::rand()%100 ) );
	wifiHelper.SetRemoteStationManager("ns3::AarfWifiManager");
		//install on AP
		wifi_mac.SetType("ns3::ApWifiMac",
			"Ssid", SsidValue(ssid));
		apDevice = wifiHelper.Install(wifi_phy, wifi_mac, wifiApNode);
		//install on STA
		wifi_mac.SetType("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid), "ActiveProbing", BooleanValue (false));
		staDevices = wifiHelper.Install(wifi_phy, wifi_mac, wifiStaNodes);

	/* WiFi Mobility */
	mobility.SetPositionAllocator("ns3::GridPositionAllocator",
		"MinX",			DoubleValue(0.0),
		"MinY",			DoubleValue(0.0),
		"DeltaX",		DoubleValue(5.0),
		"DeltaY",		DoubleValue(10.0),
		"GridWidth",	UintegerValue(3),
		"LayoutType",	StringValue("RowFirst"));
		//install for AP
		mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
		mobility.Install(wifiApNode);
		//install for STA
		mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
			"Bounds", RectangleValue( Rectangle(-50, 50, -50, 50) ));
		mobility.Install(wifiStaNodes);
}