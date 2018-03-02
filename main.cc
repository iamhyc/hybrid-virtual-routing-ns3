/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Mark@Lab1112, EEE, SUSTech
 */

// {Node, Channel, Device, Stack, Address, Interface, Service, Application}
// Stack -->Node
// Channel -->Node = DeviceContainer
// Service -->Node = ApplicationContainer

#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("HybridVirtualRouting");

/*Global Static Configuration*/
static NodeContainer nodes;
static PointToPointHelper pointToPoint;
static NetDeviceContainer devices;

int main(int argc, char *argv[])
{
  bool verbose = false; 

  /* Command Parameter Parse */
  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.Parse (argc, argv);

  /* 1.Node Container Setup */
  nodes.Create (2);

  /* 2.Channel Helper Setup */
  //p2p channel
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  //wifi channel, {Area, Numbers, Range, Mobility, Loss, Delay}

  /* 1+2 Channel->Node */
  devices = pointToPoint.Install (nodes);

  /* 3.Internet Stack Install */
  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  /* 4.Application Install */
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  /* 5.Simulator Setup */
  Time::SetResolution (Time::NS);
  if (verbose)
  {
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }

  /* Run Simulator */
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
