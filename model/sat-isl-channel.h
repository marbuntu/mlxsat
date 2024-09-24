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



#ifndef SATELLITE_ISL_CHANNEL_H
#define SATELLITE_ISL_CHANNEL_H


#include <ns3/channel.h>
#include <ns3/object-base.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/propagation-loss-model.h>

#include "sat-isl-net-device.h"
#include "sat-isl-signal.h"


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

    /**
     * @brief   Transfer a Signal over the Channel
     * 
     * @param signal    Input Signal
     */
    void Transfer(Ptr<SatelliteISLSignal> signal, Ptr<NetDevice> sender);


    void SetPropagationDelayModel(Ptr<PropagationDelayModel> model);

    Ptr<PropagationDelayModel> GetPropagationDelayModel() const;


    void SetPropagationLossModel(Ptr<PropagationLossModel> model);

    Ptr<PropagationLossModel> GetPropagationLossModel() const;



    virtual void Add(Ptr<SatelliteISLNetDevice> device);


    virtual std::size_t GetNDevices() const;


    virtual Ptr<NetDevice> GetDevice(std::size_t i) const;


    Ptr<NetDevice> GetDevice(const Mac48Address addr) const;


    double EstimateGain(const Ptr<MobilityModel> tx_mob, const Ptr<MobilityModel> rx_mob, double fc) const;



private:

    uint64_t _addrToHash(const Mac48Address addr) const;
    Mac48Address _hashToAddr(const uint64_t hash) const;

    //std::vector<Ptr<SimpleNetDevice>> m_devices;
    std::unordered_map<uint64_t, Ptr<SatelliteISLNetDevice>> m_devices;
    
    double m_bandwidth;                                     //! Channel Bandwidth in Hz
    bool m_compensateDoppler;                               //! Compensate Doppler-Shift


    Ptr<PropagationDelayModel>  m_propDelay;                // Propagation Delay Model
    Ptr<PropagationLossModel>   m_propLoss;                 //! Propagation Loss Model


};


} // namespace name


#endif  /* SATELLITE_ISL_CHANNEL_H */