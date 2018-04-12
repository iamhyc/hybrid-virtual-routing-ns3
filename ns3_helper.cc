
#include "ns3_helper.h"
#include <arpa/inet.h>

using namespace ns3_helper;

// channel helper
static PointToPointHelper p2pHelper;
static CsmaHelper csmaHelper;
/*wifi channel: {Area, Numbers, Range, Mobility, Loss, Delay}*/
static YansWifiPhyHelper	wifi_phy;
static WifiMacHelper		wifi_mac;
static MobilityHelper		mobility;
static WifiHelper	wifi_helper;
static WiFiManager	wifi_manager;
// service helper
static uint32_t addr_counter = 0;
static uint32_t addr_base = inet_addr("10.0.0.0");
static InternetStackHelper stack;
static Ipv4AddressHelper address;

void ns3_helper::InstallStackHelper(Nodes& nodes)
{
	stack.Install(nodes);
}

void AddrHelper()
{
	char *ipAddr;
	struct in_addr s = {.s_addr=addr_base};
	UNUSED(addr_counter);

	ipAddr = inet_ntoa(s);
	address.SetBase (ipAddr, "255.255.255.0");
	// increment counter base
	addr_base = htobe32(be32toh(addr_base) + (1 << 8));
}

void ns3_helper::p2pBuilder(KeyPair keyword, flowSchema schema,
							int index, NodesTuple& parent, NodesTuple& child)
{
	Nets p2pNets;
	Ifaces p2pIfaces;

	auto it = parent.netsl.find(keyword);
	if(it!=parent.netsl.end())
	{
		p2pNets = it->second.first;
		p2pIfaces = it->second.second;
	}

	p2pHelper.SetDeviceAttribute("DataRate", StringValue(schema.throughput));
	p2pHelper.SetChannelAttribute("Delay", StringValue(schema.delay));
	for(uint32_t it=0; it!=child.nodes.GetN(); ++it)
	{
		Nodes tmp_node( parent.nodes.Get(index) );
		tmp_node.Add( child.nodes.Get(it) );
		Nets tmp_nets( p2pHelper.Install(tmp_node) ); //point-to-point

		AddrHelper();
		p2pNets.Add( tmp_nets );
		p2pIfaces.Add( address.Assign(tmp_nets) );
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
	wifi_helper.SetRemoteStationManager("ns3::AarfWifiManager");
		//install on AP
		wifi_mac.SetType("ns3::ApWifiMac",
			"Ssid", SsidValue(ssid));
		apDevice = wifi_helper.Install(wifi_phy, wifi_mac, wifiApNode);
		//install on STA
		wifi_mac.SetType("ns3::StaWifiMac",
			"Ssid", SsidValue (ssid), "ActiveProbing", BooleanValue (false));
		staDevices = wifi_helper.Install(wifi_phy, wifi_mac, wifiStaNodes);

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