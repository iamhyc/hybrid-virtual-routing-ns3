
#include "ns3_helper.h"

using namespace ns3_helper;

// channel helper
PointToPointHelper p2pHelper;
CsmaHelper csmaHelper;
/*wifi channel: {Area, Numbers, Range, Mobility, Loss, Delay}*/
WifiHelper wifiHelper;
WifiMacHelper wifi_mac;
MobilityHelper mobility;
WiFiManager wifi_manager;
// service helper
InternetStackHelper stack;
Ipv4AddressHelper address;

void p2pDefaultHelper(Nodes const &p2pNodes, Nets &p2pDevices, Ifaces &p2pIfaces)
{
    p2pHelper.SetDeviceAttribute( "DataRate", StringValue("5Mbps") );
    p2pHelper.SetChannelAttribute( "Delay", StringValue("2ms") );
    p2pDevices = p2pHelper.Install(p2pNodes);
}

void csmaDefaultHelper(Nodes const &csmaNodes, Nets &csmaDevices, Ifaces &csmaIfaces)
{
    csmaHelper.SetChannelAttribute( "DataRate", StringValue("100Mbps") );
    csmaHelper.SetChannelAttribute( "Delay", TimeValue(NanoSeconds (6560)) );
    csmaDevices = csmaHelper.Install(csmaNodes);
}

void wifiDefaultHelper(Nodes const &wifiNodes, Nets &wifiDevices, Ifaces &wifiIfaces)
{

}