

#include <ns3/channel.h>
#include <ns3/object-base.h>

#include <ns3/simple-net-device.h>


namespace ns3
{
    


class MegaConstellationStaticChannel : public Channel
{
public:

    static TypeId GetTypeId(void);

    MegaConstellationStaticChannel();


    virtual void Add(Ptr<NetDevice> device);


    virtual std::size_t GetNDevices() const;


    virtual Ptr<NetDevice> GetDevice(std::size_t i) const;


private:

    //std::vector<Ptr<SimpleNetDevice>> m_devices;

    double m_bandwidth;

};


} // namespace name