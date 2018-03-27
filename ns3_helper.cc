
#include "ns3_helper.h"

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
static InternetStackHelper stack;
static Ipv4AddressHelper address;

void p2pDefaultHelper(Nodes const &p2pNodes, Nets &p2pDevices, Ifaces &p2pIfaces)
{
	p2pHelper.SetDeviceAttribute( "DataRate", StringValue("5Mbps") );
	p2pHelper.SetChannelAttribute( "Delay", StringValue("2ms") );
	p2pDevices = p2pHelper.Install(p2pNodes);
}

void csmaDefaultHelper(Nodes const &csmaNodes, Nets &csmaDevices, Ifaces &csmaIfaces)
{
	csmaHelper.SetChannelAttribute( "DataRate", StringValue("100Mbps") );
	csmaHelper.SetChannelAttribute( "Delay", TimeValue(NanoSeconds(6560)) );
	csmaDevices = csmaHelper.Install(csmaNodes);
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