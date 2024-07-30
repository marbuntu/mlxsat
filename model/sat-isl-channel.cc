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
    , m_compensateDoppler(true)
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
            .AddAttribute(
                "DopplerCompensated"
                , "Adjust Center-Frequency at the transmitter to mitigate the Doppler-Shift"
                , BooleanValue(true)
                , MakeBooleanAccessor(&SatelliteISLChannel::m_compensateDoppler)
                , MakeBooleanChecker()
            )
        ;

        return tid;
    }


    // void SatelliteISLChannel::Send(Ptr<SatelliteISLSignal> signal)
    // {
    //     NS_LOG_FUNCTION(this << signal);
    // }


    void SatelliteISLChannel::Send(Ptr<Packet> pck, uint16_t protocol, Mac48Address dst, Mac48Address src, Ptr<NetDevice> sender)
    {
        /**
         * \todo Add a flag for doppler compensation. This should be done for the net-device but it might be easier at this point
         * 
         */

        NS_LOG_FUNCTION(this << pck << protocol << dst << src << sender);


        uint64_t rx_mac;
        dst.CopyTo((uint8_t*) &rx_mac);


        if (auto rxd = m_devices.find(rx_mac); rxd != m_devices.end())
        {
            NS_LOG_FUNCTION(this << rxd->first << rxd->second->GetAddress());
        }
        else
        {
            NS_LOG_ERROR("Receiver Device not attached to this Channel!");
            return;
        }


        if (m_compensateDoppler)
        {
            
        }


        for (const auto& [key, dev ] : m_devices)
        {
            if (dev == sender) continue;

            Simulator::ScheduleWithContext(
                dev->GetNode()->GetId(),
                Time("100ms"),
                &SatelliteISLNetDevice::Receive,
                dev,
                pck->Copy(),
                protocol,
                dst,
                src
            );
            // dev->Receive(pck, protocol, dst, scr);
        }
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
        return 0;
    }


    Ptr<NetDevice> SatelliteISLChannel::GetDevice(std::size_t i) const
    {
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