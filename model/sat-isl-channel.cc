/**
 * @brief   Inter-Satellite-Link Channel Model
 * 
 * @file    sat-isl-channel.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-06-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-channel.h"
#include "sat-isl-net-device.h"
#include "sat-isl-pck-tag.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"
#include "ns3/node.h"


namespace ns3 
{

    NS_LOG_COMPONENT_DEFINE("SatelliteISLChannel");
    NS_OBJECT_ENSURE_REGISTERED(SatelliteISLChannel);


    SatelliteISLChannel::SatelliteISLChannel()
    : m_bandwidth(0.0)
    // , m_compensateDoppler(true)
    , m_propDelay()
    , m_propLoss()
    {
    }


    SatelliteISLChannel::~SatelliteISLChannel()
    {
    }


    TypeId SatelliteISLChannel::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SatelliteStaticISLChannel")
            .SetParent<Channel>()
            .SetGroupName("Network")
            .AddConstructor<SatelliteISLChannel>()
            .AddAttribute(
                "Bandwidth"
                , "Channel Bandwidth in Hz"
                , DoubleValue(500e6)
                , MakeDoubleAccessor(&SatelliteISLChannel::m_bandwidth)
                , MakeDoubleChecker<double>(0.0)
            )
            // .AddAttribute(
            //     "DopplerShift"
            //     , "Center-Frequency at the Receiver the Doppler-Shift"
            //     , BooleanValue(true)
            //     , MakeBooleanAccessor(&SatelliteISLChannel::m_compensateDoppler)
            //     , MakeBooleanChecker()
            // )
            .AddAttribute(
                "NoiseTemperature"
                , "Equivalent Noise Temperature in Kelvin"
                , DoubleValue(1000.0)
                , MakeDoubleAccessor(&SatelliteISLChannel::SetNoiseTemperature, &SatelliteISLChannel::GetNoiseTemperature)
                , MakeDoubleChecker<double>(1.0)
            )
        ;

        return tid;
    }


    void SatelliteISLChannel::Send(Ptr<Packet> pck, uint16_t protocol, Mac48Address dst, Mac48Address src, Ptr<NetDevice> sender)
    {
        /**
         * \todo Add a flag for doppler compensation. This should be done for the net-device but it might be easier at this point
         * 
         */

        NS_LOG_FUNCTION(this << "\t" << pck << "\t" << protocol << "\t" << dst << "\t" << src << "\t" << sender);


        ISLPacketTag tag;
        if (!pck->PeekPacketTag(tag))
        {
            NS_LOG_FUNCTION(this << "Critical Error - No Pck Tag assigned!");
            return;
        }

        Ptr<NetDevice> other = nullptr;
        if (dst.IsBroadcast())
        {
            other = GetDevice(tag.GetSilentDst());
        }
        else
        {
            other = GetDevice(dst);
        }

        if (other == nullptr)
        {
            NS_LOG_ERROR("Error - Device not connected to the Channel!");
            return;
        }


        if (m_propDelay == nullptr) 
        {
            NS_LOG_ERROR("Critical Error - No Delay model aggregated to Channel!");
            return;
        }

        Ptr<MobilityModel> tx_mob = sender->GetNode()->GetObject<MobilityModel>();
        Ptr<MobilityModel> rx_mob = other->GetNode()->GetObject<MobilityModel>();
        Time delay = m_propDelay->GetDelay(tx_mob, rx_mob);

        Ptr<SatelliteISLNetDevice> dev = StaticCast<SatelliteISLNetDevice>(other);

        Simulator::ScheduleWithContext(
            dev->GetNode()->GetId(),
            delay,
            &SatelliteISLNetDevice::Receive,
            dev,
            pck->Copy(),
            protocol,
            dst,
            src
        );
    }


    void SatelliteISLChannel::SetPropagationDelayModel(Ptr<PropagationDelayModel> model)
    {
        NS_LOG_FUNCTION(this << model);
        m_propDelay = model;
    }


    Ptr<PropagationDelayModel> SatelliteISLChannel::GetPropagationDelayModel() const
    {
        NS_LOG_FUNCTION(this);
        return m_propDelay;
    }


    void SatelliteISLChannel::SetPropagationLossModel(Ptr<PropagationLossModel> model)
    {
        NS_LOG_FUNCTION(this << model);
        m_propLoss = model;
    }


    Ptr<PropagationLossModel> SatelliteISLChannel::GetPropagationLossModel() const
    {
        NS_LOG_FUNCTION(this);
        return m_propLoss;
    }


    void SatelliteISLChannel::Add(Ptr<SatelliteISLNetDevice> device)
    {

        NS_LOG_FUNCTION(this << device << device->GetAddress());

        uint64_t mac ;
        device->GetAddress().CopyTo((uint8_t*) &mac);
        m_devices.insert(std::pair{mac, device});
        //m_devices.push_back(device);
    }


    std::size_t SatelliteISLChannel::GetNDevices() const
    {
        return m_devices.size();
    }


    std::unordered_map<uint64_t, Ptr<SatelliteISLNetDevice>>::iterator SatelliteISLChannel::GetDevicesBegin()
    {
        return m_devices.begin();
    }


    std::unordered_map<uint64_t, Ptr<SatelliteISLNetDevice>>::iterator SatelliteISLChannel::GetDevicesEnd()
    {
        return m_devices.end();
    }


    Ptr<NetDevice> SatelliteISLChannel::GetDevice(std::size_t i) const
    {   
        if (i >= m_devices.size()) return nullptr;

        auto it = m_devices.begin();
        size_t n = 0;
        while(it != m_devices.end())
        {
            if (n == i) return it->second;
            it++;
            n++;
        }

        return nullptr;
    }


    Ptr<NetDevice> SatelliteISLChannel::GetDevice(const Mac48Address addr) const
    {
        NS_LOG_FUNCTION(this << addr);

        if (auto rxd = m_devices.find(_addrToHash(addr)); rxd != m_devices.end())
        {
            return rxd->second;
        }

        return nullptr;
    }


    double SatelliteISLChannel::EstimateGain(const Ptr<MobilityModel> tx_mob, const Ptr<MobilityModel> rx_mob, double fc) const
    {
        //m_propLoss->f

        return m_propLoss->CalcRxPower(0, tx_mob, rx_mob);
    }


    void SatelliteISLChannel::SetNoiseTemperature(const double temp)
    {
        if (temp <= 0.0) return;
        m_noiseTemp = temp;
    }


    double SatelliteISLChannel::GetNoiseTemperature() const
    {
        return m_noiseTemp;
    }


    uint64_t SatelliteISLChannel::_addrToHash(const Mac48Address addr) const
    {
        uint64_t hash;
        addr.CopyTo((uint8_t*) &hash);
        return hash;
    }


    Mac48Address SatelliteISLChannel::_hashToAddr(const uint64_t hash) const
    {
        Mac48Address addr;
        addr.CopyFrom((uint8_t*) &hash);
        return addr;
    }




}   /* namespace ns-3   */