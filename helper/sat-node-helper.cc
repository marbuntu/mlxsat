/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    sat-node-helper.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-10-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-node-helper.h"


namespace ns3
{

    TypeId SatelliteNodeHelper::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteNodeHelper")
            .SetParent<Object>()
            .AddConstructor<SatelliteNodeHelper>()
        ;

        return tid;
    }


    SatelliteNodeHelper::SatelliteNodeHelper()
    : m_itfFactory(DefaultISLInterfaceSetup::GetDefaultFactory(DefaultISLInterfaceSetup::SYMMETRIC_4x))
    {
    }


    SatelliteNodeHelper::~SatelliteNodeHelper()
    {
    }


    void SatelliteNodeHelper::SetInterfaceAttribute(std::string attr, const AttributeValue &value)
    {
        m_itfFactory.SetAttribute(attr, value);
    }


    Ptr<SatelliteISLInterfaceHelper> SatelliteNodeHelper::GetInterfaceFactory()
    {
        return Ptr<SatelliteISLInterfaceHelper>(&m_itfFactory);
    }


    Ptr<Node> SatelliteNodeHelper::Create() const
    {
        Ptr<Node> node = CreateObject<Node>();

        Ptr<SatelliteISLNetDevice> itf = m_itfFactory.Create();
        itf->SetNode(node);
        node->AddDevice(itf);

        return node;
    }


    void SatelliteNodeHelper::printSatNodeInfo(std::ostream& out, const Ptr<Node> node)
    {
        Ptr<SatelliteISLNetDevice> itf = StaticCast<SatelliteISLNetDevice>(node->GetDevice(0));

        out << "Test Sat\n";

        if (itf != nullptr)
        {
            out << "> Terminals: " << itf->GetNTerminals();
        }
        else out << "No Satellite ISL Interface found \n";

        out << "\n";
    }


}   /* namespace ns3 */