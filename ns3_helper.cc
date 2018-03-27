
#include "ns3_helper.h"

using namespace ns3_helper;

// channel helper
PointToPointHelper p2p;
CsmaHelper csma;
/*wifi channel: {Area, Numbers, Range, Mobility, Loss, Delay}*/
WifiHelper wifi;
WiFiManager wifi_manager;
WifiMacHelper wifi_mac;
MobilityHelper mobility;
// service helper
InternetStackHelper stack;
Ipv4AddressHelper address;

void p2pDefaultHelper(Nodes const &p2pNodes, Nets &p2pDevices, Ifaces &p2pIfaces)
{

}

void csmaDefaultHelper(Nodes const &csmaNodes, Nets &csmaDevices, Ifaces &csmaIfaces)
{

}

void wifiDefaultHelper(Nodes const &wifiNodes, Nets &wifiDevices, Ifaces &wifiIfaces)
{

}