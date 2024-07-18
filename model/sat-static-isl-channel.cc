


#include "sat-static-isl-channel.h"
#include "sat-isl-net-device.h"

#include "ns3/double.h"
#include "ns3/simulator.h"
#include "ns3/node.h"


namespace ns3 
{

NS_LOG_COMPONENT_DEFINE("MegaConstStaticChannel");
NS_OBJECT_ENSURE_REGISTERED(SatelliteStaticISLChannel);


    TypeId SatelliteStaticISLChannel::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SatelliteStaticISLChannel")
            .SetParent<Channel>()
            .SetGroupName("Network")
            .AddConstructor<SatelliteStaticISLChannel>()
            .AddAttribute(
                "Bandwidth"
                , "Channel Bandwidth in Hz"
                , DoubleValue(500e6)
                , MakeDoubleAccessor(&SatelliteStaticISLChannel::m_bandwidth)
                , MakeDoubleChecker<double>(0.0)
            )
        ;

        return tid;
    }


    void SatelliteStaticISLChannel::Send(Ptr<Packet> pck, uint16_t protocol, Mac48Address dst, Mac48Address src, Ptr<NetDevice> sender)
    {
        NS_LOG_FUNCTION(this << pck << protocol << dst << src << sender);

        for (const Ptr<SatelliteISLNetDevice> &dev : m_devices)
        {
            if (dev == sender) continue;

            Simulator::ScheduleWithContext(
                dev->GetNode()->GetId(),
                Time("100ms"),
                &SatelliteISLNetDevice::Receive,
                dev,
                pck->Copy(),
                protocol,
                dst,
                src
            );
            // dev->Receive(pck, protocol, dst, scr);
        }
    }


    void SatelliteStaticISLChannel::Add(Ptr<SatelliteISLNetDevice> device)
    {
        m_devices.push_back(device);
    }


    SatelliteStaticISLChannel::SatelliteStaticISLChannel()
    {

    }


    std::size_t SatelliteStaticISLChannel::GetNDevices() const
    {
        return 0;
    }


    Ptr<NetDevice> SatelliteStaticISLChannel::GetDevice(std::size_t i) const
    {
        return nullptr;
    }



}   /* namespace ns-3   */