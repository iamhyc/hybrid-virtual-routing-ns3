
#include "ns3_helper.h"
#include <arpa/inet.h>

using namespace std;
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
static uint32_t addr_base = inet_addr("10.0.0.0");
static uint32_t metric_base = 5;
static InternetStackHelper stack;
static Ipv4AddressHelper address;

void ns3_helper::InstallStackHelper(Nodes& nodes)
{
	stack.Install(nodes);
}

void IpAddrHelper(int num=0)
{
	char *ipAddr;
	struct in_addr s = {.s_addr=addr_base};
	UNUSED(metric_base);

	ipAddr = inet_ntoa(s);
	address.SetBase (ipAddr, "255.255.255.0");
	// increment counter base
	addr_base = htobe32(be32toh(addr_base) + (1 << 8));
}

void ns3_helper::mobilityRepeater(std::string name, wifiSchema& schema, int mobility[4])
{
	static map<string, int> repeater;
	auto item = repeater.find(name);
	if(item!=repeater.end())
	{
		int iter = item->second/4;
		++repeater[name];
		switch (item->second%4)
		{
		case 1:
			schema.mobility[0] = mobility[0]*iter;
			schema.mobility[1] = - mobility[1]*iter;
			break;
		case 2:
			schema.mobility[0] = - mobility[1]*iter;
			schema.mobility[1] = mobility[1]*iter;
			break;
		case 3:
			schema.mobility[0] = - mobility[0]*iter;
			schema.mobility[1] = - mobility[1]*iter;
			break;
		default:
			schema.mobility[0] = mobility[0]*iter;
			schema.mobility[1] = mobility[1]*iter;
			break;
		}
	}
	else
	{
		repeater[name] = 0;
	}
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

		IpAddrHelper();
		p2pNets.Add( tmp_nets );
		p2pIfaces.Add( address.Assign(tmp_nets) );
	}
	parent.netsl[keyword] = std::make_pair(p2pNets, p2pIfaces);
}

void ns3_helper::csmaBuilder(KeyPair keyword, flowSchema schema, NodesTuple& parent, NodesTuple& child)
{
	csmaHelper.SetChannelAttribute("DataRate", StringValue(schema.throughput));
	csmaHelper.SetChannelAttribute("Delay", StringValue(schema.delay));
	// csmaDevices = csmaHelper.Install(csmaNodes);
}

void ns3_helper::wifiBuilder(KeyPair keyword, wifiSchema schema,
							int index, NodesTuple& parent, NodesTuple& child)
{
	Nodes apNode( parent.nodes.Get(index) );
	Nodes staNodes( child.nodes );
	Nets apDevice, staDevices, wifiDevices;
	Ifaces wifiIfaces;

	auto it = parent.netsl.find(keyword);
	if(it!=parent.netsl.end())
	{
		wifiDevices = it->second.first;
		wifiIfaces = it->second.second;
	}

	/* Yans Physical Layer */
	wifi_phy = YansWifiPhyHelper::Default();
	wifi_phy.SetChannel( YansWifiChannelHelper::Default().Create() );
	
	/* WiFi MAC Layer */
	Ssid ssid = Ssid( schema.ssid );
	wifi_helper.SetRemoteStationManager("ns3::AarfWifiManager");
	IpAddrHelper();

	/* install on AP */
	wifi_mac.SetType("ns3::ApWifiMac",
					"Ssid", SsidValue(ssid));
	apDevice = wifi_helper.Install(wifi_phy, wifi_mac, apNode);
	mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
		"MinX", 		DoubleValue(schema.mobility[0]),
		"MinY", 		DoubleValue(schema.mobility[1]));
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility.Install(apNode);

	/* install on STA */
	wifi_mac.SetType("ns3::StaWifiMac",
					"Ssid", SsidValue (ssid),
					"ActiveProbing", BooleanValue (false));
	staDevices = wifi_helper.Install(wifi_phy, wifi_mac, staNodes);
	mobility.SetPositionAllocator("ns3::UniformDiscPositionAllocator",
		"X",			DoubleValue(schema.mobility[0]),
		"Y",			DoubleValue(schema.mobility[1]),
		"rho",			DoubleValue(schema.mobility[2]));
	double len = sqrt(2.0)*schema.mobility[2];
	mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
			"Bounds", RectangleValue(Rectangle( \
				schema.mobility[0]-len, schema.mobility[0]+len,
				schema.mobility[1]-len, schema.mobility[1]+len)));
	mobility.Install(staNodes);

	/* join together */
	wifiDevices.Add(apDevice);
	wifiDevices.Add(staDevices);
	wifiIfaces = address.Assign(wifiDevices);
	parent.netsl[keyword] = std::make_pair(wifiDevices, wifiIfaces);
}
