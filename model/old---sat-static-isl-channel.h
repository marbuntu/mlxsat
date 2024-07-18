/**
 * @brief   
 * 
 * @file    sat-static-isl-channel.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 0.1
 * @date    2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <ns3/channel.h>
#include <ns3/object-base.h>

#include "sat-net-device.h"


namespace ns3
{

class SatelliteStaticISLChannel : public Channel
{
public:

    /**
     * @brief Get the Type Id object
     * 
     * @return Object TypeId 
     */
    static TypeId GetTypeId(void);

    SatelliteStaticISLChannel();


    void Send(Ptr<Packet> pck, uint16_t protocol, Mac48Address dst, Mac48Address src, Ptr<NetDevice> sender);


    virtual void Add(Ptr<NetDevice> device);


    virtual std::size_t GetNDevices() const;


    virtual Ptr<NetDevice> GetDevice(std::size_t i) const;

private:

    std::vector<Ptr<NetDevice>> m_device;
    double m_bandwidth;


};  /* SatelliteStaticISLChannel */


}   /* namespace ns3 */