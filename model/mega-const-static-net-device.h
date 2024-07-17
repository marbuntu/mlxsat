

#include <ns3/net-device.h>


namespace ns3
{


class MegaConstellationStaticNetDevice : NetDevice
{
public:
    
    static TypeId GetTypeId();

    
    MegaConstellationStaticNetDevice();

    
    void Receive(Ptr<Packet> packet, Mac48Address src, Mac48Address dst);

    void SetChannel(Ptr<Channel> channel);

    void SetAddress(Mac48Address address);

private:
    /**
     * To keep compatability with IP Stack
     */
    Mac48Address m_address;
    
    Ptr<Channel> m_channel;


};  /* MegaConstellationStaticNetDevice */


} /* namespace ns3 */