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


namespace ns3
{
    //std::vector<Ptr<SatelliteNodeTag>> SatelliteNodeTag::m_globalNodes;


    TypeId SatelliteNodeTag::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteNodeTag")
            .SetParent<Object>()
        ;

        return tid;
    }


    SatelliteNodeTag::SatelliteNodeTag(size_t ConstellationID, size_t Orbit, size_t SatNo)
    {
    }


    SatelliteNodeTag::~SatelliteNodeTag()
    {
    }



}   /* namespace ns3 */