

#include <ns3/core-module.h>
#include <ns3/node-container.h>
#include <ns3/walker-constellation-helper.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/sat-isl-interface-helper.h>
#include <ns3/sat-isl-channel.h>

#include <ns3/internet-module.h>
#include <ns3/applications-module.h>


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("satellite-tcp-traffic");


int main(int argc, char* argv[])
{

    Time interval (Seconds(100));
    Time duration (Seconds(60.0 * 10.0));

    LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
    LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
    LogComponentEnable("Socket", LOG_LEVEL_ALL);




    Ptr<WalkerConstellationHelper> helper = CreateObjectWithAttributes<WalkerConstellationHelper>(
        "Inclination", DoubleValue(66.0),
        "NumOfSats", IntegerValue(10),
        "NumOfOrbits", IntegerValue(10),
        "Altitude", DoubleValue(480)
    );


    helper->Initialize();

    /* Select Nodes */
    NodeContainer nodes;
    nodes.Add(helper->getSatellite(2 * 10 + 5)->GetObject<Node>());       // Sat 0
    nodes.Add(helper->getSatellite(2 * 10 + 6)->GetObject<Node>());       // Sat 1
    nodes.Add(helper->getSatellite(1 * 10 + 5)->GetObject<Node>());       // Sat 2
    nodes.Add(helper->getSatellite(2 * 10 + 4)->GetObject<Node>());       // Sat 3
    nodes.Add(helper->getSatellite(3 * 10 + 5)->GetObject<Node>());       // Sat 4


    /* Create Channel Model */
    Ptr<SatelliteISLChannel> channel = CreateObjectWithAttributes<SatelliteISLChannel>();
    Ptr<FriisPropagationLossModel> loss_model = CreateObject<FriisPropagationLossModel>();
    channel->SetPropagationLossModel(loss_model);


    SatelliteISLInterfaceHelper itf_helper = DefaultISLInterfaceSetup::GetDefaultFactory(DefaultISLInterfaceSetup::SYMMETRIC_4x);
    NetDeviceContainer devs = itf_helper.Install(nodes, channel);


    InternetStackHelper internet;
    internet.Install(nodes);

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i = ipv4.Assign(devs);
    Address serverAddress = Address(i.GetAddress(2));


    NS_LOG_INFO("Create UdpServer application on node 1.");
    uint16_t port = 4000;
    UdpServerHelper server(port);
    ApplicationContainer apps = server.Install(nodes.Get(2));
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(10.0));


    NS_LOG_INFO("Create UdpClient application on node 0 to send to node 1.");
    uint32_t MaxPacketSize = 1024;
    Time interPacketInterval = Seconds(0.05);
    uint32_t maxPacketCount = 320;
    UdpClientHelper client(serverAddress, port);
    client.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    client.SetAttribute("Interval", TimeValue(interPacketInterval));
    client.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));
    apps = client.Install(nodes.Get(0));
    apps.Start(Seconds(2.0));
    apps.Stop(Seconds(10.0));


    NS_LOG_UNCOND("Starting Simulator...");
    Simulator::Run();
    Simulator::Destroy();


    return 0;
}