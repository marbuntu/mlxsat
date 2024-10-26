/**
 * @brief   Satellite Node Tag for additional Information
 * 
 * @file    sat-node-tag.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-26-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_NODE_TAG_H
#define SATELLITE_NODE_TAG_H

#include "ns3/object.h"
#include <unordered_map>


namespace ns3
{

    class SatelliteNodeTag : public Object
    {
    public:

        static TypeId GetTypeId();


        SatelliteNodeTag(size_t ConstellationID, size_t Orbit, size_t SatNo);
        ~SatelliteNodeTag();


    protected:

        static std::vector<Ptr<SatelliteNodeTag>> m_globalNodes;


    };  /* SatelliteNodeTag */


};  /* namespace ns3 */


#endif /* SATELLITE_NODE_TAG_H */