/* -*- Mode:C++; c-file-style:"gnu"; -*- */
/*
 * Mark@Lab1112, EEE, SUSTech
 * support following topology scenario;
 * **Router**:	interconnect with CSMA,		 equipped with WiFi-AP and P2P
 * **Relay**:	interconnect with Wi-Fi Sta, equipped with P2P
 * 1. single-layer-wifi
 * 2. vlc-p2p-relay-wifi
 * 3. vlc-p2p-relay-wifi-hybrid
 * 4. vlc-p2m-relay-wifi
 * 5. wifi-p2p-relay-wifi
 */
#include <unistd.h>
#include <iostream>

#include "ns3_net.h"
#include "ns3_helper.h"
#include "ns3_perf.h"

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("HybridVirtualRouting");

namespace ns3_net
{
	void NetRootTree::applyApplications()
	{
		rapidjson::Value* application = GetValueByPointer(*this->doc, "/application");
		UNUSED(application);
	}
}

int main(int argc, char *argv[])
{
	/* Command Parameter Parse */
	bool quiet = false;
	CommandLine cmd;
	cmd.AddValue("quiet", "Tell echo applications to log if true", quiet);
	cmd.Parse(argc, argv);

	/* {Node, Channel, Device, Stack, Address, Interface, Service, Application}
	 * // 0.Set default attributes and random seed
	 * // 1.Node Container Setup
	 * 		using ns3::NodeContainer
	 * 		Create(); Add(); Get();
	 * // 2.Channel Helper Setup
	 * 		helper.Install(NodeContainer);
	 * // 3.Internet Stack Install
	 * 		stack.install(NodeContainer)
	 * 		address.setBase("Interface", "NetMask");
	 * 		address.Assign(NetDeviceContainer);
	 * // 4.Application Install
	 * // 5.Connect trace sources and sinks
	*/
	auto json_file = "examples/main/json/test.json";
	ns3_net::NetRootTree rt(json_file);
	// rt.printLayers();

	//6.Simulator Setup 
	Time::SetResolution(Time::NS);
	if (!quiet)
	{
		// ChannelHelper.EnablePcapAll("main");
		// LogComponentEnable("UdpApplication", LOG_LEVEL_INFO);
	}

	/* Run Simulator */
	Simulator::Run(); // paralleled thread running later
	Simulator::Destroy();
	return 0;
}