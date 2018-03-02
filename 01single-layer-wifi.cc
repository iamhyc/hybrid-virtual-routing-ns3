/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Mark@Lab1112, EEE, SUSTech
 */

#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SingleLayerWifi");

int main(int argc, char *argv[])
{
	/* Command Parameter Parse */
	CommandLine cmd;
	cmd.Parse (argc, argv);

	/* Run Simulator */
	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}