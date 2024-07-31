/**
 * @brief   Inter Satellite Net-Device 
 * 
 * @file    sat-isl-net-device.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 0.1
 * @date    2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-net-device.h"
#include "sat-isl-channel.h"
#include "sat-isl-pck-tag.h"

#include "ns3/simulator.h"
#include "ns3/pointer.h"
#include "ns3/node.h"
#include "ns3/error-model.h"
#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/trace-source-accessor.h"

#include "ns3/mobility-model.h"


namespace ns3
{

NS_LOG_COMPONENT_DEFINE("SatelliteISLNetDevice");


    TypeId SatelliteISLNetDevice::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLNetDevice")
            .SetParent<NetDevice>()
            .SetGroupName("Network")
            .AddConstructor<SatelliteISLNetDevice>()
            .AddAttribute(
                "ReceiveErrorModel",
                "Receiver Error Model for Pck Loss",
                PointerValue(),
                MakePointerAccessor(&SatelliteISLNetDevice::m_recErrModel),
                MakePointerChecker<ErrorModel>()
            )
            .AddAttribute(
                "TxQueue",
                "NetDevice Transmit Queue",
                StringValue("ns3::DropTailQueue<Packet>"),
                MakePointerAccessor(&SatelliteISLNetDevice::m_queue),
                MakePointerChecker<Queue<Packet>>()
            )
            .AddAttribute(
                "ATPDelay",
                "Delay in [ms] to account for Aquisition, Pointing and Tracking, to align Rx/Tx Antennas",
                DoubleValue(0.0),
                MakeDoubleAccessor(&SatelliteISLNetDevice::m_atpDelay),
                MakeDoubleChecker<double>(0.0)
            )
            .AddTraceSource(
                "PhyRxDrop",
                "Trace Source to indicate Packet Loss at the Device",
                MakeTraceSourceAccessor(&SatelliteISLNetDevice::m_phyRxDropTrace),
                "ns3::Packet::TracedCallback"
            )
        ;


        return tid;
    }


    SatelliteISLNetDevice::SatelliteISLNetDevice() 
    : m_channel(nullptr)
    , m_node(nullptr)
    , m_recErrModel(nullptr)
    {
        NS_LOG_FUNCTION(this);
    }



    void SatelliteISLNetDevice::Receive(Ptr<Packet> packet, uint16_t protocol, Mac48Address dst, Mac48Address src)
    {
        NS_LOG_FUNCTION(this << packet << protocol << dst << src);
        NetDevice::PacketType pckType;

        if (m_recErrModel && m_recErrModel->IsCorrupt(packet))
        {
            m_phyRxDropTrace(packet);
            return;
        }

        if (dst == m_address)
        {
            pckType = NetDevice::PACKET_HOST;
        }
        else if (dst.IsBroadcast())
        {
            pckType = NetDevice::PACKET_BROADCAST;
        }
        else if (dst.IsGroup())
        {
            pckType = NetDevice::PACKET_MULTICAST;
        }
        else
        {
            pckType = NetDevice::PACKET_OTHERHOST;
        }

        if (pckType != NetDevice::PACKET_OTHERHOST)
        {
            m_rxCallback(this, packet, protocol, src);
        }

        if (!m_promiscCallback.IsNull())
        {
            m_promiscCallback(this, packet, protocol, src, dst, pckType);
        }
    }


    void SatelliteISLNetDevice::SetChannel(Ptr<SatelliteISLChannel> channel)
    {
        NS_LOG_FUNCTION(this << channel);
        m_channel = channel;
        m_channel->Add(this);
        m_linkUp = true;
        m_linkChangeCallbacks();
    }


    void SatelliteISLNetDevice::SetQueue(Ptr<Queue<Packet>> queue)
    {
        NS_LOG_FUNCTION(this << queue);
        m_queue = queue;
    }


    Ptr<Queue<Packet>> SatelliteISLNetDevice::GetQueue() const
    {
        NS_LOG_FUNCTION(this);
        return m_queue;
    }


    void SatelliteISLNetDevice::SetReceiveErrorModel(Ptr<ErrorModel> em)
    {
        NS_LOG_FUNCTION(this << em);
        m_recErrModel = em;
    }


    void SatelliteISLNetDevice::SetIfIndex(const uint32_t idx) 
    {
        NS_LOG_FUNCTION(this << idx);
        m_ifIndex = idx;
    }


    uint32_t SatelliteISLNetDevice::GetIfIndex() const
    {
        NS_LOG_FUNCTION(this);
        return m_ifIndex;
    }


    Ptr<Channel> SatelliteISLNetDevice::GetChannel() const
    {
        NS_LOG_FUNCTION(this);
        return m_channel;
    }


    void SatelliteISLNetDevice::SetAddress(Address address)
    {
        NS_LOG_FUNCTION(this << address);
        m_address = Mac48Address::ConvertFrom(address);
    }


    Address SatelliteISLNetDevice::GetAddress() const
    {
        NS_LOG_FUNCTION(this);
        return m_address;
    }


    bool SatelliteISLNetDevice::SetMtu(const uint16_t mtu)
    {
        NS_LOG_FUNCTION(this << mtu);
        m_mtu = mtu;
        return true;
    }


    uint16_t SatelliteISLNetDevice::GetMtu() const
    {
        NS_LOG_FUNCTION(this);
        return m_mtu;
    }


    bool SatelliteISLNetDevice::IsLinkUp() const
    {
        NS_LOG_FUNCTION(this);
        return m_linkUp;
    }


    void SatelliteISLNetDevice::AddLinkChangeCallback(Callback<void> clbk)
    {
        NS_LOG_FUNCTION(this << &clbk);
        m_linkChangeCallbacks.ConnectWithoutContext(clbk);
    }


    bool SatelliteISLNetDevice::IsBroadcast() const
    {
        NS_LOG_FUNCTION(this);
        if (m_pointToPointMode)
        {
            return false;
        }

        return true;
    }


    Address SatelliteISLNetDevice::GetBroadcast() const
    {
        NS_LOG_FUNCTION(this);
        return Mac48Address("ff:ff:ff:ff:ff:ff");
    }


    bool SatelliteISLNetDevice::IsMulticast() const
    {
        NS_LOG_FUNCTION(this);
        if (m_pointToPointMode)
        {
            return false;
        }

        return true;
    }


    Address SatelliteISLNetDevice::GetMulticast(Ipv4Address multicastGroup) const
    {
        NS_LOG_FUNCTION(this << multicastGroup);
        return Mac48Address::GetMulticast(multicastGroup);
    }


    Address SatelliteISLNetDevice::GetMulticast(Ipv6Address addr) const
    {
        NS_LOG_FUNCTION(this << addr);
        return Mac48Address::GetMulticast(addr);
    }


    bool SatelliteISLNetDevice::IsPointToPoint() const
    {
        NS_LOG_FUNCTION(this);
        if (m_pointToPointMode)
        {
            return true;
        }

        return false;
    }


    bool SatelliteISLNetDevice::IsBridge() const
    {
        NS_LOG_FUNCTION(this);
        return false;
    }


    bool SatelliteISLNetDevice::Send(Ptr<Packet> packet, const Address& dst, uint16_t protocolNumber)
    {
        NS_LOG_FUNCTION(this << packet << dst << protocolNumber);
        return SendFrom(packet, m_address, dst, protocolNumber);
    }


    bool SatelliteISLNetDevice::SendFrom(Ptr<Packet> pck, const Address& src, const Address& dst, uint16_t protocolNumber)
    {
        NS_LOG_FUNCTION(this << pck << src << dst << protocolNumber);
        
        if (pck->GetSize() > GetMtu())
        {
            return false;
        }

        Mac48Address mac_dst = Mac48Address::ConvertFrom(dst);
        Mac48Address mac_src = Mac48Address::ConvertFrom(src);

        ISLPacketTag tag;
        tag.SetSrc(mac_src);
        tag.SetDst(mac_dst);
        tag.SetProto(protocolNumber);

        pck->AddPacketTag(tag);

        if (m_queue->Enqueue(pck))
        {
            if (m_queue->GetNPackets() >= 1 && !m_finishTransmissionEvent.IsRunning())
            {
                StartTransmission();
            }
            return true;
        }

        return false;
    }


    void SatelliteISLNetDevice::StartTransmission()
    {
        if (m_queue->GetNPackets() == 0)
        {
            return;
        }

        NS_ASSERT_MSG(!m_finishTransmissionEvent.IsRunning(), "Transmission already in Progress!");

        Ptr<MobilityModel> mob = m_node->GetObject<MobilityModel>();

        NS_LOG_FUNCTION(this << mob << mob->GetPosition());

        Ptr<Packet> pck = m_queue->Dequeue();

        DataRate rate = DataRate("1Mb/s");
        Time txTime = rate.CalculateBytesTxTime(pck->GetSize());

        m_finishTransmissionEvent = Simulator::Schedule(txTime, &SatelliteISLNetDevice::FinishTransmission, this, pck);
    }


    void SatelliteISLNetDevice::FinishTransmission(Ptr<Packet> pck)
    {
        NS_LOG_FUNCTION(this);

        ISLPacketTag tag;
        pck->RemovePacketTag(tag);

        Mac48Address src = tag.GetSrc();
        Mac48Address dst = tag.GetDst();
        uint16_t proto = tag.GetProto();

        m_channel->Send(pck, proto, dst, src, this);

        StartTransmission();
    }


    Ptr<Node> SatelliteISLNetDevice::GetNode() const
    {
        NS_LOG_FUNCTION(this);
        return m_node;
    }


    void SatelliteISLNetDevice::SetNode(Ptr<Node> node)
    {
        NS_LOG_FUNCTION(this << node);
        m_node = node;
    }


    bool SatelliteISLNetDevice::NeedsArp() const
    {
        NS_LOG_FUNCTION(this);
        if (m_pointToPointMode)
        {
            return false;
        }

        return true;
    }


    void SatelliteISLNetDevice::SetReceiveCallback(NetDevice::ReceiveCallback clbk)
    {
        NS_LOG_FUNCTION(this << &clbk);
        m_rxCallback = clbk;
    }


    void SatelliteISLNetDevice::DoDispose()
    {
        NS_LOG_FUNCTION(this);
        m_channel = nullptr;
        m_node = nullptr;
        m_recErrModel = nullptr;
        m_queue->Dispose();
        
        /** 
         * \todo Finish Transmission Event
        */

        NetDevice::DoDispose();
    }


    void SatelliteISLNetDevice::SetPromiscReceiveCallback(PromiscReceiveCallback clbk)
    {
        NS_LOG_FUNCTION(this << &clbk);
        m_promiscCallback = clbk;
    }


    bool SatelliteISLNetDevice::SupportsSendFrom() const
    {
        NS_LOG_FUNCTION(this);
        return true;
    }


    Mac48Address SatelliteISLNetDevice::GetMacAddress() const
    {
        return m_address;
    }


}   /* namespace ns3 */