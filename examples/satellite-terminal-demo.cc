
#include <ns3/core-module.h>
#include <ns3/node-container.h>
#include <ns3/walker-constellation-helper.h>
#include <ns3/sat-isl-channel.h>
#include <ns3/sat-isl-net-device.h>
#include <ns3/sat-isl-terminal.h>
#include <ns3/sat-isl-antenna.h>
#include <ns3/cosine-antenna-model.h>

using namespace ns3;



// * 
// *   Setup of 5 Satllite, in the following pattern
// *
// *     x       1       x
// *     |       |       |
// *     |       |       |         /
// *     2 ----- 0 ----- 4        /||  Direction of Motion
// *     |       |       |         ||
// *     |       |       |         ||
// *     x       3       x
// *
// *   Sat 0, 1, 3 in Orbit 2
// *   Sat 2       in Orbit 1
// *   Sat 4       in Orbit 3


typedef struct {
    NodeContainer*  nodes;
    Ptr<Channel>        channel;

    std::ofstream *output;

    size_t cnt;

} sim_params_t;


static void test_transmission(sim_params_t *params)
{

    NodeContainer *nodes = params->nodes;

    Ptr<Packet> pck = Create<Packet>(500);
    //nodes->Get(2)->GetDevice(0)->Send(pck, nodes->Get(1)->GetDevice(0)->GetAddress(), 0);

    Ptr<SatelliteISLChannel> sat_chn = StaticCast<SatelliteISLChannel>(params->channel);

    // Extract Center Satellite
    Ptr<SatelliteISLNetDevice> center = StaticCast<SatelliteISLNetDevice>(nodes->Get(0)->GetDevice(0));
    Ptr<MobilityModel> tx_mob = nodes->Get(0)->GetObject<MobilityModel>();

    Ptr<MobilityModel> sat4_mob = nodes->Get(4)->GetObject<MobilityModel>();

    // Create the local Reference
    Ptr<LVLHReference> ref = CreateObject<LVLHReference>();
    ref->UpdateLocalReference(tx_mob->GetPosition(), tx_mob->GetVelocity());


    *params->output << params->cnt;

    // Repeat for each Terminal
    for (size_t n = 0; n < 4; n++)
    {
        Ptr<SatelliteISLTerminal> ter = center->GetISLTerminal(n);
        Ptr<AntennaModel> ant = ter->GetAntennaModel();

        // Update Reference
        ter->SetLocalReference(ref);
        Angles tx_angles = ter->GetRelativeAngles(sat4_mob->GetPosition());

        //DataRate rate = ter->GetAntennaGain(tx_mob, sat4_mob); //ter->GetRateEstimation(tx_mob, sat4_mob, sat_chn->GetPropagationLossModel());
        
        double gain = ant->GetGainDb(tx_angles);

        *params->output << "\t" << tx_angles.GetAzimuth() << "\t" << tx_angles.GetInclination() << "\t" << gain;
    }

    *params->output << "\n";
    params->cnt++;
}



static void register_interface(Ptr<Node> node, Ptr<SatelliteISLChannel> channel)
{
    Ptr<SatelliteISLNetDevice> itf = CreateObject<SatelliteISLNetDevice>();

    // Ptr<CosineAntennaModel> antenna = CreateObjectWithAttributes<CosineAntennaModel>(
    //     "MaxGain", DoubleValue(100),
    //     "VerticalBeamwidth", DoubleValue(120),
    //     "HorizontalBeamwidth", DoubleValue(45),
    //     "Orientation", DoubleValue(90.0)
    // );

    Ptr<SatelliteISLAntenna> antenna = CreateObjectWithAttributes<SatelliteISLAntenna>(
        "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
        "MaxGainDbi", DoubleValue(0.0),
        "OpeningAngle", DoubleValue(160.0)
    );


    Ptr<SatelliteISLTerminal> t_north = CreateObject<SatelliteISLTerminal>();
    t_north->SetAntennaModel(antenna);
    t_north->SetRelativeOrientation(0, 0, 0);
    itf->RegisterISLTerminal(t_north);

    Ptr<SatelliteISLTerminal> t_east = CreateObject<SatelliteISLTerminal>();
    t_east->SetAntennaModel(antenna);
    t_east->SetRelativeOrientation(0, 0, 90.0);
    itf->RegisterISLTerminal(t_east);

    Ptr<SatelliteISLTerminal> t_south = CreateObject<SatelliteISLTerminal>();
    t_south->SetAntennaModel(antenna);
    t_south->SetRelativeOrientation(0, 0, 180.0);
    itf->RegisterISLTerminal(t_south);

    Ptr<SatelliteISLTerminal> t_west = CreateObject<SatelliteISLTerminal>();
    t_west->SetAntennaModel(antenna);
    t_west->SetRelativeOrientation(0, 0, 270.0);
    itf->RegisterISLTerminal(t_west);


    itf->SetAddress(Mac48Address::Allocate());
    itf->SetChannel(channel);
    itf->SetNode(node);
    node->AddDevice(itf);
}



int main(int argc, char* argv[])
{
    // LogComponentEnable("SatelliteISLNetDevice", LOG_LEVEL_ALL);
    // LogComponentEnable("SatelliteISLChannel", LOG_LEVEL_ALL);
    LogComponentEnable("SatelliteISLTerminal", LOG_LEVEL_ALL);

    // Create a Constellation and extract some satellites
    Ptr<WalkerConstellationHelper> helper = CreateObjectWithAttributes<WalkerConstellationHelper>(
        "Inclination", DoubleValue(66.0),
        "NumOfSats", IntegerValue(10),
        "NumOfOrbits", IntegerValue(10),
        "Altitude", DoubleValue(480)
    );

    helper->Initialize();

    NodeContainer nodes;


    nodes.Add(helper->getSatellite(2 * 10 + 5)->GetObject<Node>());       // Sat 0
    nodes.Add(helper->getSatellite(2 * 10 + 6)->GetObject<Node>());       // Sat 1
    nodes.Add(helper->getSatellite(1 * 10 + 5)->GetObject<Node>());       // Sat 2
    nodes.Add(helper->getSatellite(2 * 10 + 4)->GetObject<Node>());       // Sat 3
    nodes.Add(helper->getSatellite(3 * 10 + 5)->GetObject<Node>());       // Sat 4


    // Create Channel Model
    Ptr<SatelliteISLChannel> channel = CreateObjectWithAttributes<SatelliteISLChannel>();
    Ptr<FriisPropagationLossModel> loss_model = CreateObject<FriisPropagationLossModel>();
    channel->SetPropagationLossModel(loss_model);


    // The Constellion Helper automatically associates a Node with each Satellite
    // So we only need to setup the corresponding Net Devices and Terminals

    for (NodeContainer::Iterator it = nodes.Begin(); it != nodes.End(); it++)
    {
        register_interface(*it, channel);
    }


    // Setup the Simulation:

    std::ofstream out("./terminal-test.txt");
    //out << "Testing \n";

    sim_params_t params = {
        .nodes = &nodes,
        .channel = channel,
        .output = &out,
        .cnt = 0
    };

    size_t N = 100;
    Time step = Seconds(60);

    for (size_t n = 0; n < N; n++)
    {
        Simulator::Schedule(step * n, test_transmission, &params);
    
    }


    NS_LOG_UNCOND("Starting Simulator...");
    Simulator::Run();

    NS_LOG_UNCOND("Clear Simulator..");
    Simulator::Destroy();

    out.close();

    return 0;
}