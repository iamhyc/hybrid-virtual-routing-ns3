
#include "ns3_helper.h"

using namespace ns3_helper;

// channel helper
PointToPointHelper p2p;
CsmaHelper csma;
/*wifi channel: {Area, Numbers, Range, Mobility, Loss, Delay}*/
WiFiManager wifi_manager;
WifiHelper wifi;
WifiMacHelper wifi_mac;
MobilityHelper mobility;
// service helper
InternetStackHelper stack;
Ipv4AddressHelper address;
