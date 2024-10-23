/**
 * @brief   Satellite ISL Packet Tag
 * 
 * @file    sat-isl-pck-tag.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_ISL_PCK_TAG_H
#define SATELLITE_ISL_PCK_TAG_H

#include "ns3/tag.h"
#include "ns3/mac48-address.h"

namespace ns3
{

class ISLPacketTag : public Tag
{
public:
    /**
     * @brief Get the TypeId
     * 
     * @return Object TypeId 
     */
    static TypeId GetTypeId();
    TypeId GetInstanceTypeId() const override;

    uint32_t GetSerializedSize() const override;
    void Serialize(TagBuffer i) const override;
    void Deserialize(TagBuffer i) override;


    /**
     * Set the source address
     * \param src source address
     */
    void SetSrc(Mac48Address src);
    
    /**
     * Get the source address
     * \return the source address
     */
    Mac48Address GetSrc() const;

    /**
     * Set the destination address
     * \param dst destination address
     */
    void SetDst(Mac48Address dst);
    
    /**
     * Get the destination address
     * \return the destination address
     */
    Mac48Address GetDst() const;

    /**
     * Set the protocol number
     * \param proto protocol number
     */
    void SetProto(uint16_t proto);
    /**
     * Get the protocol number
     * \return the protocol number
     */
    uint16_t GetProto() const;


    void SetSilentDst(Mac48Address);


    Mac48Address GetSilentDst() const;


    void Print(std::ostream& os) const override;

private:

    Mac48Address m_src;        //!< source address
    Mac48Address m_dst;        //!< destination address
    uint16_t m_proto;          //!< protocol number

    Mac48Address m_silentdst;  //!< Silent Destination Address used for Broadcasting

};  /* ISLPacketTag */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_PCK_TAG_H */