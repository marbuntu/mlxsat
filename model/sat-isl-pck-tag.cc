/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    _template.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-pck-tag.h"

#include "ns3/log.h"


namespace ns3
{
    NS_LOG_COMPONENT_DEFINE("ISLPacketTag");

    NS_OBJECT_ENSURE_REGISTERED(ISLPacketTag);



    TypeId ISLPacketTag::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::ISLPacketTag")
            .SetParent<Tag>()
            .SetGroupName("Network")
            .AddConstructor<ISLPacketTag>();
    
        return tid;
    }


    TypeId ISLPacketTag::GetInstanceTypeId() const
    {
        return GetTypeId();
    }


    uint32_t ISLPacketTag::GetSerializedSize() const
    {
        return 8+8+2;
    }


    void ISLPacketTag::Serialize(TagBuffer i) const
    {
        uint8_t buff[12];
        m_src.CopyTo(&buff[0]);
        m_dst.CopyTo(&buff[6]);
        i.Write(buff, 12);
        i.WriteU16(m_proto);
    }


    void ISLPacketTag::Deserialize(TagBuffer i)
    {
        uint8_t buff[12];
        i.Read(buff, 12);
        m_src.CopyFrom(&buff[0]);
        m_dst.CopyFrom(&buff[6]);
        m_proto = i.ReadU16();
    }


    void ISLPacketTag::SetSrc(Mac48Address src)
    {
        NS_LOG_FUNCTION(this << src);
        m_src = src;
    }


    Mac48Address ISLPacketTag::GetSrc() const 
    {
        NS_LOG_FUNCTION(this);
        return m_src;
    }


    void ISLPacketTag::SetDst(Mac48Address dst)
    {
        NS_LOG_FUNCTION(this << dst);
        m_dst = dst;
    }


    Mac48Address ISLPacketTag::GetDst() const
    {
        NS_LOG_FUNCTION(this);
        return m_dst;
    }


    void ISLPacketTag::SetProto(uint16_t proto)
    {
        NS_LOG_FUNCTION(this << proto);
        m_proto = proto;
    }


    uint16_t ISLPacketTag::GetProto() const
    {
        NS_LOG_FUNCTION(this);
        return m_proto;
    }


    void ISLPacketTag::Print(std::ostream& os) const
    {
        os << "src=" << m_src << " dst=" << m_dst << " proto=" << m_proto;
    }


}   /* namespace ns3 */