
#include <ns3/core-module.h>
#include <ns3/node-container.h>
#include <ns3/walker-constellation-helper.h>
#include <ns3/sat-isl-channel.h>
#include <ns3/sat-isl-antenna.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/object-factory.h>

#include <ns3/sat-isl-interface-helper.h>

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
    NodeContainer *nodes;
    Ptr<Channel> channel;

    std::ofstream *output;

    size_t cnt;

} sim_params_t;



static const uint32_t MTU_bytes = 9000U;


static double DbmToW(const double value_dbm)
{
    double mw = std::pow(10.0, value_dbm / 10.0);
    return mw / 1000.0;
}



/**
 * Try to plot the channel gain w.r.t. the change in velocity
 * Show the variation of intra-orbit links
 * Show the variation of inter-orbit links  
 */
static void plot_step(sim_params_t *params)
{

    NodeContainer *nodes = params->nodes;

    Ptr<SatelliteISLChannel> channel = StaticCast<SatelliteISLChannel>(params->channel);
    Ptr<PropagationLossModel> loss = channel->GetPropagationLossModel();

    Ptr<SatelliteISLNetDevice> center = StaticCast<SatelliteISLNetDevice>(nodes->Get(0)->GetDevice(0));

    Ptr<MobilityModel> tx_mob = nodes->Get(0)->GetObject<MobilityModel>();
    Ptr<MobilityModel> rx_mob = nodes->Get(2)->GetObject<MobilityModel>();

    double dist = tx_mob->GetDistanceFrom(rx_mob);

    double loss_db = loss->CalcRxPower(0, tx_mob, rx_mob);
    double loss_f = DbmToW(loss_db);

    
    // Create the local Reference
    Ptr<LVLHReference> ref = CreateObject<LVLHReference>();
    ref->UpdateLocalReference(tx_mob->GetPosition(), tx_mob->GetVelocity());
    center->SetLocalReference(ref);

    *params->output << params->cnt << "\t" << dist << "\t" << loss_f << "\t" << loss_db;


    for (auto it = center->BeginTerminals(); it != center->EndTerminals(); it++)
    {
        Ptr<SatelliteISLTerminal> ter = *it;

        DataRate rate = ter->GetRateEstimation(tx_mob, rx_mob, loss);
        uint32_t Tt = 0;

        if (rate.GetBitRate() > 0)
        {
            Tt = rate.CalculateBytesTxTime(MTU_bytes).GetSeconds();
        }

        *params->output << "\t" << rate.GetBitRate() << "\t" << Tt; // << rate.CalculateBytesTxTime(MTU_bytes).GetSeconds();

    }

    *params->output << "\n";

    params->cnt++;
}



int main(int argc, char* argv[])
{
    SatelliteISLInterfaceHelper itf_helper = DefaultISLInterfaceSetup::GetDefaultFactory(
        DefaultISLInterfaceSetup::SYMMETRIC_4x
    );

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

    for (NodeContainer::Iterator it = nodes.Begin(); it != nodes.End(); it++)
    {
        Ptr<SatelliteISLNetDevice> itf = itf_helper.CreateAndAggregate(*it, channel);
    }


    /* Simulation Setup */
    std::ofstream outp("./com-test.txt");

    sim_params_t params = {
        .nodes = &nodes,
        .channel = channel,
        .output = &outp,
        .cnt = 0
    };


    size_t N = 24 * 60;
    Time step = Seconds(10);
    for (size_t n = 0; n < N; n++)
    {
        Simulator::Schedule(step * n, plot_step, &params);
    }


    NS_LOG_UNCOND("Starting Simulator...");
    Simulator::Run();

    NS_LOG_UNCOND("Clear Simulator...");
    Simulator::Destroy();

    return 0;
}