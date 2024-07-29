/**
 * @brief   Inter-Satellite-Link Channel Model
 * 
 * @file    sat-isl-channel.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-06-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <ns3/channel.h>
#include <ns3/object-base.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/propagation-loss-model.h>

#include "sat-isl-net-device.h"


namespace ns3
{
    
/**
 * \ingroup satellite
 * 
 * @brief 
 */
class SatelliteISLChannel : public Channel
{
public:

    static TypeId GetTypeId(void);


    SatelliteISLChannel();
    ~SatelliteISLChannel();


    void Send(Ptr<Packet> pck, uint16_t protocol, Mac48Address dst, Mac48Address src, Ptr<NetDevice> sender);

    //void Send(Ptr<Packet> pck, uint16_t protocol, Ptr<NetDevice> src, Ptr<Node> dst);


    void SetPropagationDelayModel(Ptr<PropagationDelayModel> model);

    Ptr<PropagationDelayModel> GetPropagationDelayModel() const;


    void SetPropagationLossModel(Ptr<PropagationLossModel> model);

    Ptr<PropagationLossModel> GetPropagationLossModel() const;



    virtual void Add(Ptr<SatelliteISLNetDevice> device);


    virtual std::size_t GetNDevices() const;


    virtual Ptr<NetDevice> GetDevice(std::size_t i) const;


private:

    //std::vector<Ptr<SimpleNetDevice>> m_devices;
    std::unordered_map<uint64_t, Ptr<SatelliteISLNetDevice>> m_devices;
    
    double m_bandwidth;                                     //! Channel Bandwidth in Hz
    bool m_compensateDoppler;                               //! Compensate Doppler-Shift


    Ptr<PropagationDelayModel>  m_propDelay;                // Propagation Delay Model
    Ptr<PropagationLossModel>   m_propLoss;                 //! Propagation Loss Model


};


} // namespace name