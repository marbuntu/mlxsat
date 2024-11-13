/**
 * @brief   Satellite Interconnection Table
 * 
 * @file    sat-isl-intercon-table.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-11-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_ISL_INTERCON_TABLE_H
#define SATELLITE_ISL_INTERCON_TABLE_H


#include "ns3/object.h"
#include "ns3/object-base.h"

#include <map>
#include "sat-isl-def.h"



namespace ns3
{
    typedef std::multimap<satid_t, satid_t> interconMatrix_t;
    typedef std::multimap<satid_t, satid_t>::iterator interconMatrixIter_t;

    class SatISLInterconTable : public Object
    {
    public:
        static TypeId GetTypeId();


        SatISLInterconTable();
        ~SatISLInterconTable();


        void Add(const satid_t src, const satid_t dst) const;
        void Remove(const satid_t src, const satid_t dst) const;
        void RemoveAll(const satid_t src) const;

        std::pair<interconMatrixIter_t, interconMatrixIter_t> GetKnownNeighbours(const satid_t id);

        bool IsAvailable(const satid_t src, const satid_t dst);

        size_t GetSize() const;

        void Plot(std::ostream &out);

    protected:

        static std::multimap<satid_t, satid_t> m_interconMap;

    }; /* SatISLInterconTable */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_INTERCON_TABLE_H */