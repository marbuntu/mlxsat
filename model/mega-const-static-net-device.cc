

#include "mega-const-static-net-device.h"


namespace ns3
{

    TypeId MegaConstellationStaticNetDevice::GetTypeId()
    {

        static TypeId tid = TypeId("ns3::MegaConstellationStaticNetDevice")
            .SetParent<NetDevice>()
            .SetGroupName("Network")
            .AddConstructor<MegaConstellationStaticNetDevice>()
        ;

        return tid;
    }


    MegaConstellationStaticNetDevice::MegaConstellationStaticNetDevice()
    {

    }


    void MegaConstellationStaticNetDevice::Receive(Ptr<Packet> packet, Mac48Address src, Mac48Address dst)
    {
        if ( dst == m_address || dst == Mac48Address::GetBroadcast())
        {

        }

    }


    void MegaConstellationStaticNetDevice::SetChannel(Ptr<Channel> channel)
    {
        m_channel = channel;
        //m_channel->Add(this);
    }


    void MegaConstellationStaticNetDevice::SetAddress(Mac48Address address)
    {
        m_address = address;
    }


}   /* namespace ns3 */