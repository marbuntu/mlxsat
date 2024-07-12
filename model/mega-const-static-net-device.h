

#include <ns3/net-device.h>


namespace ns3
{


class MegaConstellationStaticNetDevice : NetDevice
{
public:
    
    static TypeId GetTypeId();

    MegaConstellationStaticNetDevice();

    void Receive(Ptr<Packet> packet, Mac48Address src, Mac48Address dst);

    void SetChannelAttribute(Ptr<Channel> channel);

    void SetAddress(Mac48Address address);

private:

    Mac48Address m_address;


};  /* MegaConstellationStaticNetDevice */


} /* namespace ns3 */