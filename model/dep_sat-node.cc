/**
 * @brief   Satellite Node
 * 
 * @file    sat-node.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-09-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-node.h"


namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("SatelliteNodeHelper");
    NS_OBJECT_ENSURE_REGISTERED(SatelliteNodeHelper);


    TypeId SatelliteNodeHelper::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SatelliteNodeHelper")
            .SetParent<Object>()
            .AddConstructor<SatelliteNodeHelper>()
        ;

        return tid;
    }


    TypeId SatelliteNodeHelper::GetInstanceTypeId() const
    {
        return GetTypeId();
    }


    SatelliteNodeHelper::SatelliteNodeHelper()
    : m_node(CreateObject<Node>())
    {
    }


    SatelliteNodeHelper::~SatelliteNodeHelper()
    {
    }



    size_t SatelliteNodeHelper::AddISLTerminal(const std::string uid, Ptr<SatelliteISLTerminal> terminal)
    {
        //m_node->AddDevice(terminal->GetNetDevice());
        m_terminals.insert({uid, terminal});
        return m_terminals.size();
    }


}   /* namespace ns3 */