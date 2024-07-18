


#include "sat-static-isl-channel.h"

#include <ns3/double.h>


namespace ns3 
{

NS_LOG_COMPONENT_DEFINE("MegaConstStaticChannel");
NS_OBJECT_ENSURE_REGISTERED(SatelliteStaticISLChannel);


    TypeId SatelliteStaticISLChannel::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::MegaConstellationStaticChannel")
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
        for (const auto &dev : m_devices)
        {
            if (dev == sender) continue;

            // dev->Receive(pck, protocol, dst, scr);
        }
    }


    void SatelliteStaticISLChannel::Add(Ptr<NetDevice> device)
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