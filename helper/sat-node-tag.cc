/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    _template.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-node-tag.h"
#include "ns3/integer.h"

namespace ns3
{
    std::unordered_map<uint8_t, std::vector<uint16_t>> SatelliteNodeTag::m_CmapO;
    std::unordered_map<uint16_t, std::vector<uint16_t>> SatelliteNodeTag::m_OmapS;
    std::unordered_map<uint16_t, Ptr<SatelliteNodeTag>> SatelliteNodeTag::m_globalNodes;


    TypeId SatelliteNodeTag::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteNodeTag")
            .SetParent<Object>()
            .AddConstructor<SatelliteNodeTag>()
                    .AddAttribute("ConstellationID",
                      "Constellation ID ( 1 - 255 )",
                      IntegerValue(1),
                      MakeIntegerAccessor(&SatelliteNodeTag::m_CID),
                      MakeIntegerChecker<uint8_t>(1, 255)
            )
            .AddAttribute("OrbitID",
                        "Orbit ID ( 1 - 2^16 )",
                        IntegerValue(1),
                        MakeIntegerAccessor(&SatelliteNodeTag::m_OID),
                        MakeIntegerChecker<uint16_t>(1)
            )
        ;

        return tid;
    }


    uint16_t SatelliteNodeTag::GetConstellationsN()
    {
        return (uint16_t) m_CmapO.size();
    }


    uint16_t SatelliteNodeTag::GetOrbitsN()
    {
        return (uint16_t) m_OmapS.size();
    }


    uint16_t SatelliteNodeTag::GetOrbitsN(const uint8_t constellationID)
    {
        if (auto search = m_CmapO.find(constellationID); search != m_CmapO.end())
        {
            return search->second.size();
        }

        return 0;
    }


    uint16_t SatelliteNodeTag::GetSatsN()
    {
        return m_globalNodes.size();
    }


    uint16_t SatelliteNodeTag::GetSatsN(const uint16_t orbitID)
    {
        if (auto search = m_OmapS.find(orbitID); search != m_OmapS.end())
        {
            return search->second.size();
        }

        return 0;
    }


    std::vector<uint16_t> SatelliteNodeTag::SatsByOrbit(const uint16_t orbitID)
    {
        if (auto sat_vec = m_OmapS.find(orbitID); sat_vec != m_OmapS.end())
        {
            return sat_vec->second;
        }

        return std::vector<uint16_t>();
    }


    std::vector<uint16_t> SatelliteNodeTag::OrbitsByConstellation(const uint8_t constellationID)
    {
        if (auto orb_vec = m_CmapO.find(constellationID); orb_vec != m_CmapO.end())
        {
            return orb_vec->second;
        }

        return std::vector<uint16_t>();
    }


    Ptr<Node> SatelliteNodeTag::GetSatellite(const uint16_t id)
    {
        if (auto sat = m_globalNodes.find(id); sat != m_globalNodes.end())
        {
            return sat->second->GetObject<Node>();
        }

        return nullptr;
    }


    SatelliteNodeTag::SatelliteNodeTag()
    {
    }


    SatelliteNodeTag::~SatelliteNodeTag()
    {
    }


    uint16_t SatelliteNodeTag::GetId() const
    {
        return m_id;
    }


    uint8_t SatelliteNodeTag::GetCID() const
    {
        return m_CID;
    }


    uint16_t SatelliteNodeTag::GetOID() const
    {
        return m_OID;
    }


    void SatelliteNodeTag::Register()
    {
        m_id = m_globalNodes.size() + 1;

        while (m_globalNodes.find(m_id) != m_globalNodes.end())
        {
            m_id++;
        }
        m_globalNodes.insert(std::pair{m_id, Ptr<SatelliteNodeTag>(this)});

        // Register Satellite to Orbit
        auto orb_vec = m_OmapS.find(m_OID);
        if (orb_vec != m_OmapS.end())
        {
            // Is Known Orbit
            orb_vec->second.push_back(m_id);
        }
        else 
        {
            // Register New Orbit
            m_OmapS.insert(std::pair{m_OID, std::vector<uint16_t>({m_id})});

            // Register Orbit to Constellation
            auto cst_vec = m_CmapO.find(m_CID);

            if (cst_vec != m_CmapO.end())
            {
                // Is Known Constellation
                cst_vec->second.push_back(m_OID);
            }
            else
            {
                m_CmapO.insert(std::pair{m_CID, std::vector<uint16_t>({m_OID})});
            }
        }
    }


    void SatelliteNodeTag::Register(const uint8_t constellationID, const uint16_t orbitID)
    {
        m_CID = constellationID;
        m_OID = orbitID;
        Register();
    }


}   /* namespace ns3 */