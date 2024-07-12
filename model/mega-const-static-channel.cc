


#include "mega-const-static-channel.h"

#include <ns3/double.h>


namespace ns3 
{

NS_LOG_COMPONENT_DEFINE("MegaConstStaticChannel");
NS_OBJECT_ENSURE_REGISTERED(MegaConstellationStaticChannel);


    TypeId MegaConstellationStaticChannel::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::MegaConstellationStaticChannel")
            .SetParent<Channel>()
            .SetGroupName("Network")
            .AddConstructor<MegaConstellationStaticChannel>()
            .AddAttribute(
                "Bandwidth"
                , "Channel Bandwidth in Hz"
                , DoubleValue(500e6)
                , MakeDoubleAccessor(&MegaConstellationStaticChannel::m_bandwidth)
                , MakeDoubleChecker<double>(0.0)
            )
        ;

        return tid;
    }


    MegaConstellationStaticChannel::MegaConstellationStaticChannel()
    {

    }


    std::size_t MegaConstellationStaticChannel::GetNDevices() const
    {
        return 0;
    }


    Ptr<NetDevice> MegaConstellationStaticChannel::GetDevice(std::size_t i) const
    {
        return nullptr;
    }



}   /* namespace ns-3   */