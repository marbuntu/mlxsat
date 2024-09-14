


#include <ns3/core-module.h>
#include <ns3/enum.h>

#include "ns3/sat-isl-terminal.h"
#include "ns3/sat-isl-channel.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/sat-leo-propagation-loss.h"
#include "ns3/sat-isl-antenna.h"
#include "ns3/friis-spectrum-propagation-loss.h"

#include "math.h"

using namespace ns3;


//static ns3::OrientationHelperTestSuite g_Suite("mlxsat-one", "mod-example");


typedef struct {

    Ptr<SatelliteISLTerminal> t1;
    Ptr<SatelliteISLTerminal> t2;

    Ptr<MobilityModel> sat1;
    Ptr<MobilityModel> sat2;

}   simpram_t;


// static void CreateSatellite(Ptr<SatelliteISLChannel> channel)
// {



// }

Vector toUnitVector(Vector vec)
{
    double len = vec.GetLength();
    if (len != 0) {
        return Vector(vec.x / len, vec.y / len, vec.z / len);
    }

    return Vector(0, 0, 0);
}

double vectorDotProduct(Vector vec1, Vector vec2)
{
    if ((vec1.GetLength() * vec2.GetLength()) == 0) return INFINITY;

    return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z) / (vec1.GetLength() * vec2.GetLength());
}


/**
 * @brief   Calculate the Angle of Arrival
 * 
 * @param tx_ant    Normal Vector of the Transmit Antenna
 * @param rx_ant    Normal Vector of the Receiver Antenna
 * @return double   Azimuth Angle Phi
 */
double CalcAoA(Vector tx_ant, Vector rx_ant)
{
    Vector diff = rx_ant - tx_ant;
    return acos(diff.x / (sqrt(diff.x * diff.x + diff.y * diff.y)));
}


double CalcAoA(Ptr<MobilityModel> mob_tx, Ptr<MobilityModel> mob_rx)
{

    Vector un1 = toUnitVector(mob_tx->GetPosition());
    Vector un2 = toUnitVector(mob_rx->GetPosition());

    Vector ant1 = Vector(0, 0, 1);

    Vector diff = un2 - un1;

    NS_LOG_UNCOND(diff);
    NS_LOG_UNCOND(vectorDotProduct(un2, un1));
    //NS_LOG_UNCOND(Vector(diff.x * ant1.x, diff.y * ant1.y, diff.z * ant1.z));

    double phi = acos(diff.x / (sqrt(diff.x * diff.x + diff.y * diff.y)));

    NS_LOG_UNCOND(180 * (phi / M_PI));


    return 0.0;
}

double CalcAoD(Ptr<MobilityModel> mob_tx, Ptr<MobilityModel> mob_rx)
{
    Vector v_tx = toUnitVector(mob_tx->GetVelocity());
    Vector p_rx = toUnitVector(mob_rx->GetPosition() - mob_tx->GetPosition());

    printf("DotProd:\t (x: %.02f, y: %.02f, z: %.02f) \t %.03f\n", p_rx.x, p_rx.y, p_rx.z, 90 - 180.0 * (vectorDotProduct(v_tx, p_rx) / M_PI));


    return 0.0;
}



static void calc_orientation(simpram_t *pram)
{
    //CalcAoA(pram->sat1, pram->sat2);
    //CalcAoD(pram->sat1, pram->sat2);

    //pram->t1->test(pram->t2);

}



int main(int argc, char *argv[])
{
    // Enable Logging:
    LogComponentEnable("SatelliteISLTerminal", LOG_LEVEL_ALL);
    LogComponentEnable("SatelliteISLChannel", LOG_LEVEL_ALL);

    // Create the ISL Channel
    Ptr<SatellitePropagationLossLEO> loss = CreateObjectWithAttributes<SatellitePropagationLossLEO>();

    Ptr<FriisPropagationLossModel> friis = CreateObjectWithAttributes<FriisPropagationLossModel>();

    Ptr<SatelliteISLChannel> channel = CreateObject<SatelliteISLChannel>();
    channel->SetPropagationLossModel(friis);


    // Create Satellite 1
    // with a predefined NetDevice:
    Ptr<SatelliteISLNetDevice> net1 = CreateObject<SatelliteISLNetDevice>();
    net1->SetAddress(Mac48Address::Allocate());

    Ptr<SatelliteISLTerminal> t1 = CreateObjectWithAttributes<SatelliteISLTerminal>(
        "ISLType", EnumValue(SatelliteISLTerminal::PointToPoint)
    );

    Ptr<SatelliteISLAntenna> ant1 = CreateObjectWithAttributes<SatelliteISLAntenna>(
        "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
        "MaxGainDbi", DoubleValue(25.0)
    );
    t1->SetAntennaModel(ant1);


    Ptr<ConstantVelocityMobilityModel> mob1 = CreateObjectWithAttributes<ConstantVelocityMobilityModel>();
    mob1->SetPosition(Vector(-1e3, -1e3, -1e3));
    mob1->SetVelocity(Vector(0, 0, 10));

    net1->SetChannel(channel);
    net1->AggregateObject(mob1);
    t1->SetupSharedInterface(net1);


    // Create Satellite 2
    // with auto-setup NetDevice:
    Ptr<ConstantPositionMobilityModel> mob2 = CreateObjectWithAttributes<ConstantPositionMobilityModel>();
    mob2->SetPosition(Vector(0, 0, 0));

    Ptr<SatelliteISLTerminal> t2 = CreateObjectWithAttributes<SatelliteISLTerminal>(
        "ISLType", EnumValue(SatelliteISLTerminal::PointToPoint)
    );

    t2->SetupInternalInterface(channel, mob2, Mac48Address::Allocate());


    Vector or1 = Vector(0, 0, 1);
    Angles an = Angles(or1);

//    CalcAoA(mob1, mob2);

    simpram_t params = 
    {
        .t1 = t1,
        .t2 = t2,
        .sat1 = mob1,
        .sat2 = mob2
    };


    Time t_step = Seconds(10);

    for (size_t t = 0; t < 20; t++)
    {
        Simulator::Schedule(t_step * t, &calc_orientation, &params);
    }


    NS_LOG_UNCOND("\n\n Starting Simulation... \n");


    Simulator::Run();
    Simulator::Destroy();


    /*
    * \todo: NetDevice, Channel and Terminal must all be aggregated with each other in order to work properly
    * else there will be a deref to nullptr error at some point!
    */

    // if(t1->IsLinkUp(Mac48Address::ConvertFrom(net2->GetAddress())))
    // {
    //     NS_LOG_UNCOND("Link is UP");
    // }
    // else
    // {
    //     NS_LOG_UNCOND("Link is DOWN");
    // }


    // t1->SetRelativeOrientation(an);


    // if(t1->IsLinkUp(t2->GetNetDevice()->GetMacAddress()))
    // {
    //     NS_LOG_UNCOND("Link is UP");
    // }
    // else
    // {
    //     NS_LOG_UNCOND("Link is DOWN");
    // }





    return 0;
}