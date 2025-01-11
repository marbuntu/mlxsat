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
            .AddAttribute(
                "MTU",
                "MTU Length in Bytes",
                IntegerValue(4000),
                MakeIntegerAccessor(&SatelliteISLNetDevice::SetMtu, &SatelliteISLNetDevice::GetMtu),
                MakeIntegerChecker<uint16_t>(1500)
            )
            .AddAttribute(
                "MinDR",
                "Data Rate the Link must achieve to be evaluated as up!",
                DataRateValue(10e3),
                MakeDataRateAccessor(&SatelliteISLNetDevice::SetMinDR, &SatelliteISLNetDevice::GetMinDR),
                MakeDataRateChecker()
            )
            .AddAttribute(
                "RxSensitivityDbm",
                "Receiver Sensitivity in Dbm",
                DoubleValue(130.0),
                MakeDoubleAccessor(&SatelliteISLNetDevice::SetRxSensitivity, &SatelliteISLNetDevice::GetRxSensitivity),
                MakeDoubleChecker<double>(0.0)
            )
            .AddAttribute(
                "GlobalICM",
                "Use Global Interconnect Matrix (ICM) for Known Neighbours",
                BooleanValue(false),
                MakeBooleanAccessor(&SatelliteISLNetDevice::m_useICM),
                MakeBooleanChecker()
            )
            .AddTraceSource(
                "PhyRxDrop",
                "Trace Source to indicate Packet Loss at the Device",
                MakeTraceSourceAccessor(&SatelliteISLNetDevice::m_phyRxDropTrace),
                "ns3::Packet::TracedCallback"
            )
            .AddTraceSource(
                "PhyRxPck",
                "Trace Source to indicate Packet Reception at the Device",
                MakeTraceSourceAccessor(&SatelliteISLNetDevice::m_phyRxTrace),
                "ns3::Packet::TracedCallback"
            )
        ;


        return tid;
    }


    SatelliteISLNetDevice::SatelliteISLNetDevice() 
    : m_channel(nullptr)
    , m_node(nullptr)
    , m_recErrModel(nullptr)
    , m_pointToPointMode(false)
    {
        NS_LOG_FUNCTION(this);
        m_refLVLH = CreateObject<LVLHReference>();
    }


    void SatelliteISLNetDevice::Receive(Ptr<Packet> packet, uint16_t protocol, Mac48Address dst, Mac48Address src)
    {
        //NS_LOG_FUNCTION(this << "\t" << packet << "\t" << protocol << "\t" << dst << "\t" << src);
        NetDevice::PacketType pckType;

        if (m_recErrModel && m_recErrModel->IsCorrupt(packet))
        {
            m_phyRxDropTrace(packet);
            return;
        }

        // Rx Trace Callback
        m_phyRxTrace(packet);


        ISLPacketTag tag;
        packet->RemovePacketTag(tag);
        

        if (dst == m_address)
        {
            pckType = NetDevice::PACKET_HOST;
        }
        else if (dst.IsBroadcast())
        {
            pckType = NetDevice::PACKET_HOST;
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
            NS_LOG_FUNCTION(this << "dst" << dst << "src" << src);
            m_rxCallback(this, packet, protocol, src);

            if (!m_promiscCallback.IsNull())
            {
                m_promiscCallback(this, packet, protocol, src, dst, pckType);
            }
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
        Ptr<NetDevice> dev_dst = m_channel->GetDevice(mac_dst);


        if ((!mac_dst.IsBroadcast()) && (dev_dst == nullptr))
        {
            NS_LOG_FUNCTION(this << "Destination Node is not attached to this Channel!");
            return false;
        } 


        if (mac_dst.IsBroadcast())
        {
            EnqueueBroadcast(pck, mac_src, mac_dst, protocolNumber);
        }
        else 
        {
            EnqueuePacket(pck, mac_src, mac_dst, protocolNumber);
        }
        

        if (m_queue->GetNPackets() >= 1 && !m_finishTransmissionEvent.IsRunning())
        {
            StartTransmission();
        }


        return false;
    }


    bool SatelliteISLNetDevice::EnqueueBroadcast(Ptr<Packet> pck, Mac48Address src, Mac48Address dst, uint16_t proto)
    {
        for(auto it = m_channel->GetDevicesBegin(); it != m_channel->GetDevicesEnd(); it++)
        {
            Ptr<NetDevice> other = it->second;
            if (other == this) continue;

            ISLPacketTag tag;
            tag.SetSrc(src);
            tag.SetDst(dst);
            tag.SetProto(proto);
            tag.SetSilentDst(Mac48Address::ConvertFrom(other->GetAddress()));

            Ptr<Packet> cpy = pck->Copy();
            cpy->AddPacketTag(tag);

            m_queue->Enqueue(cpy);
        }
        return true;
    }


    bool SatelliteISLNetDevice::EnqueuePacket(Ptr<Packet> pck, Mac48Address src, Mac48Address dst, uint16_t proto)
    {
        ISLPacketTag tag;
        tag.SetSrc(src);
        tag.SetDst(dst);
        tag.SetProto(proto);
        pck->AddPacketTag(tag);

        NS_LOG_FUNCTION(this << src << dst);

        return m_queue->Enqueue(pck);
    }


    void SatelliteISLNetDevice::StartTransmission()
    {
        if (m_queue->GetNPackets() == 0)
        {
            return;
        }

        NS_ASSERT_MSG(!m_finishTransmissionEvent.IsRunning(), "Transmission already in Progress!");
        
        
        // Deque Package
        Ptr<Packet> pck = m_queue->Dequeue();

        ISLPacketTag tag;
        if (!pck->PeekPacketTag(tag))
        {
            NS_LOG_FUNCTION(this << "Critical Error - No Pck Tag assigned!");
            Simulator::Schedule(MilliSeconds(100), &SatelliteISLNetDevice::StartTransmission, this);
            return;
        }

        Time block_time = Time(0);

        // Get Other MobilityModel
        Ptr<NetDevice> other = nullptr;

        NS_LOG_FUNCTION(this << tag.GetDst() << tag.GetSilentDst());

        if (tag.GetDst().IsBroadcast())
        {
            other = m_channel->GetDevice(tag.GetSilentDst());
        }
        else
        {
            other = m_channel->GetDevice(tag.GetDst());
        }

        if (other == nullptr)
        {
            NS_LOG_FUNCTION(this << "Critical Error - Target Device not found!");
            Simulator::Schedule(MilliSeconds(100), &SatelliteISLNetDevice::StartTransmission, this);
            return;
        }
        
        // Update Local Reference Frame
        Ptr<MobilityModel> mob = m_node->GetObject<MobilityModel>();
        m_refLVLH->UpdateLocalReference(mob->GetPosition(), mob->GetVelocity());

        DataRate rate(0);
        Ptr<SatelliteISLTerminal> term;

        for (const auto& terminal : m_terminals)
        {
            DataRate new_rate = terminal->GetRateEstimation(mob, other->GetNode()->GetObject<MobilityModel>(), m_channel->GetPropagationLossModel(), m_channel->GetNoiseTemperature());
            if ((new_rate > 0) && (new_rate > rate))
            {
                term = terminal;
                rate = new_rate;
            } 
                
        }

        if ((rate < m_minDR) || (term == nullptr))
        {
            NS_LOG_FUNCTION(this << "Critical Error - Target Device not reachable!");
            Simulator::Schedule(MilliSeconds(1), &SatelliteISLNetDevice::StartTransmission, this);
            return;
        }


        block_time = term->Transmit(pck, this, other, m_channel);

        // }

        //DataRate rate = DataRate("1Mb/s");
        //Time txTime = rate.CalculateBytesTxTime(pck->GetSize());

        //NS_LOG_UNCOND(this << "  block: " << block_time.GetMicroSeconds());

        m_finishTransmissionEvent = Simulator::Schedule(block_time, &SatelliteISLNetDevice::FinishTransmission, this, pck);
    }


    void SatelliteISLNetDevice::FinishTransmission(Ptr<Packet> pck)
    {
        NS_LOG_FUNCTION(this);
        StartTransmission();
    }


    Ptr<Node> SatelliteISLNetDevice::GetNode() const
    {
        return m_node;
    }


    void SatelliteISLNetDevice::SetNode(Ptr<Node> node)
    {
        NS_LOG_FUNCTION(this << node);
        m_node = node;
    }


    bool SatelliteISLNetDevice::NeedsArp() const
    {
        NS_LOG_FUNCTION(this << (m_pointToPointMode ? "PPP Mode" : "Routed Mode"));
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


    void SatelliteISLNetDevice::RegisterISLTerminal(const Ptr<SatelliteISLTerminal> terminal)
    {
        NS_LOG_FUNCTION(this << terminal);
        
        terminal->SetLocalReference(m_refLVLH);
        m_terminals.insert(m_terminals.end(), terminal);

    }

    Ptr<SatelliteISLTerminal> SatelliteISLNetDevice::GetISLTerminal(const size_t id) const
    {
        if (id > m_terminals.size()) return nullptr;
        return m_terminals.at(id);
    }


    void SatelliteISLNetDevice::SetLocalReference(const Ptr<LVLHReference> ref)
    {
        if (ref == nullptr) return;
        m_refLVLH = ref;

        for (auto &ter : m_terminals)
        {
            ter->SetLocalReference(ref);
        }
    }


    Ptr<LVLHReference> SatelliteISLNetDevice::GetLocalReference() const
    {
        return m_refLVLH;
    }


    void SatelliteISLNetDevice::SetMinDR(DataRate minDR)
    {
        m_minDR = minDR;
    }


    DataRate SatelliteISLNetDevice::GetMinDR() const
    {
        return m_minDR;
    }


    void SatelliteISLNetDevice::SetRxSensitivity(const double rxsens_dbm)
    {
        if (rxsens_dbm < 0.0) return;
        m_rxsensdbm = rxsens_dbm;
    }


    double SatelliteISLNetDevice::GetRxSensitivity() const
    {
        return m_rxsensdbm;
    }


    size_t SatelliteISLNetDevice::GetNTerminals() const
    {
        return m_terminals.size();
    }


}   /* namespace ns3 */