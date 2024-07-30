


#include <ns3/core-module.h>
#include <ns3/enum.h>

#include "ns3/sat-isl-terminal.h"
#include "ns3/sat-isl-channel.h"


using namespace ns3;


// static void CreateSatellite(Ptr<SatelliteISLChannel> channel)
// {



// }



int main(int argc, char *argv[])
{
    // Enable Logging:
    LogComponentEnable("SatelliteISLTerminal", LOG_LEVEL_ALL);
    LogComponentEnable("SatelliteISLChannel", LOG_LEVEL_ALL);

    // Create NetDevices for Satellite 1 & 2
    Ptr<SatelliteISLNetDevice> net1 = CreateObject<SatelliteISLNetDevice>();
    net1->SetAddress(Mac48Address::Allocate());

    Ptr<SatelliteISLNetDevice> net2 = CreateObject<SatelliteISLNetDevice>();
    net2->SetAddress(Mac48Address::Allocate());

    // Create the ISL Channel
    Ptr<SatelliteISLChannel> channel = CreateObject<SatelliteISLChannel>();


    Ptr<SatelliteISLTerminal> t1 = CreateObjectWithAttributes<SatelliteISLTerminal>(
        "ISLType", EnumValue(SatelliteISLTerminal::PointToPoint)
    );
    t1->SetNetDevice(net1);
    t1->AttachToChannel(channel);
    net1->SetChannel(channel);

    Ptr<SatelliteISLTerminal> t2 = CreateObjectWithAttributes<SatelliteISLTerminal>(
        "ISLType", EnumValue(SatelliteISLTerminal::PointToPoint)
    );
    t2->SetNetDevice(net2);


    Vector or1 = Vector(0, 0, 1);
    Angles an = Angles(or1);

    if(t1->IsLinkUp(Mac48Address::ConvertFrom(net2->GetAddress())))
    {
        NS_LOG_UNCOND("Link is UP");
    }
    else
    {
        NS_LOG_UNCOND("Link is DOWN");
    }


    t1->SetRelativeOrientation(an);
    t2->AttachToChannel(channel);
    net2->SetChannel(channel);


    if(t1->IsLinkUp(Mac48Address::ConvertFrom(net2->GetAddress())))
    {
        NS_LOG_UNCOND("Link is UP");
    }
    else
    {
        NS_LOG_UNCOND("Link is DOWN");
    }


    return 0;
}