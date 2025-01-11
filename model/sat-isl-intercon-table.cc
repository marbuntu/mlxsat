/**
 * @brief   Satellite Interconnect Table
 * 
 * @file    sat-isl-intercon-table.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-11-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#include "sat-isl-intercon-table.h"
#include "ns3/sat-node-tag.h"


namespace ns3
{
    std::multimap<satid_t, satid_t> SatISLInterconTable::m_interconMap;


    TypeId SatISLInterconTable::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatISLInterconTable")
            .SetParent<Object>()
            .AddConstructor<SatISLInterconTable>()
        ;

        return tid;
    }


    SatISLInterconTable::SatISLInterconTable()
    {
    }


    SatISLInterconTable::~SatISLInterconTable()
    {
    }


    void SatISLInterconTable::Add(const satid_t src, const satid_t dst) const
    {
        NS_ASSERT_MSG(SatelliteNodeTag::GetExists(src) && SatelliteNodeTag::GetExists(dst), "Error - Trying to assign Unknown SatID to Interconnect Matrix");

        m_interconMap.insert(std::pair{src, dst});
    }


    void SatISLInterconTable::Remove(const satid_t src, const satid_t dst) const
    {
        // @todo: Could we speed this up with std::for_each?
        
        auto cons = m_interconMap.equal_range(src);
        for(auto c = cons.first; c != cons.second; c++)
        {
            if ((*c).second == dst) m_interconMap.erase(c);
        }
    }


    void SatISLInterconTable::RemoveAll(const satid_t src) const
    {
        m_interconMap.erase(src);
    }


    size_t SatISLInterconTable::GetSize() const
    {
        return m_interconMap.size();
    }


    std::pair<interconMatrixIter_t, interconMatrixIter_t> SatISLInterconTable::GetKnownNeighbours(const satid_t id)
    {
        return m_interconMap.equal_range(id);
    }


    bool SatISLInterconTable::IsAvailable(const satid_t src, const satid_t dst)
    {
        auto Nit = m_interconMap.equal_range(src);
        for(auto &it = Nit.first; it != Nit.second; it++)
        {
            if (it->second == dst) return true;
        }

        return false;
    }


    void SatISLInterconTable::Plot(std::ostream &out)
    {
        std::for_each(m_interconMap.begin(), m_interconMap.end(), 
            [&](const auto &n) {std::cout << n.first << "\t" << n.second << "\n";} 
        );

    }

}   /* namespace ns3 */