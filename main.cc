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
// nodes
static NodeContainer      p2pNodes,   csmaNodes,    wifiApNode, wifiStaNodes;
static NetDeviceContainer p2pDevices, csmaDevices,  apDevices,  staDevices;
// channel for topology
static PointToPointHelper pointToPoint;
static CsmaHelper csma;
static WifiHelper wifi;
static WifiMacHelper mac;
static MobilityHelper mobility;
static InternetStackHelper stack;
// network service
static Ipv4InterfaceContainer p2pInterfaces, csmaInterfaces;
static Ipv4AddressHelper address;

void nodesInit()
{
  //init P2P nodes
  p2pNodes.Create(2);
  // init CSMA nodes
  csmaNodes.Add (p2pNodes.Get(1));
  csmaNodes.Create (3);
  // init wifi nodes
  wifiApNode = p2pNodes.Get (0);
  wifiStaNodes.Create(3);
}

void topologyInit()
{
  //p2p channel
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  /* 1+2 Channel->Node */
  p2pDevices = pointToPoint.Install (p2pNodes);
  //wifi channel, {Area, Numbers, Range, Mobility, Loss, Delay}
}

void networkInit()
{
  // for p2p device
  stack.Install (p2pNodes);
  address.SetBase ("10.1.1.0", "255.255.255.0");
  p2pInterfaces = address.Assign (p2pDevices);
/*  // for csma device
  stack.Install (csmaNodes);
  // for wifi device
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);*/
}

int main(int argc, char *argv[])
{
  bool verbose = false; 

  /* Command Parameter Parse */
  CommandLine cmd;
  cmd.AddValue ("verbose", "verbose and more", verbose);
  cmd.Parse (argc, argv);

  /* 1.Node Container Setup */
  nodesInit();
  /* 2.Channel Helper Setup */
  topologyInit();
  /* 3.Internet Stack Install */
  networkInit();

  /* 4.Application Install */
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (p2pNodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (p2pInterfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  /* 5.Simulator Setup */
  Time::SetResolution (Time::NS);
  if (verbose)
  {
    pointToPoint.EnablePcapAll("main-p2p");
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }

  /* Run Simulator */
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
