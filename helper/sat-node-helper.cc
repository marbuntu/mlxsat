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

#include "ns3/boolean.h"
#include "sat-node-helper.h"
#include "sat-node-tag.h"


namespace ns3
{

    TypeId SatelliteNodeHelper::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteNodeHelper")
            .SetParent<Object>()
            .AddConstructor<SatelliteNodeHelper>()
            .AddAttribute(
                "InitIPStack",
                "Initialize Nodes including the IP stack.",
                BooleanValue(true),
                MakeBooleanAccessor(&SatelliteNodeHelper::SetInitIpStack, &SatelliteNodeHelper::GetInitIpStack),
                MakeBooleanChecker()
            )
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


    void SatelliteNodeHelper::SetISLChannel(const Ptr<SatelliteISLChannel> channel)
    {
        m_channel = channel;
    }


    Ptr<SatelliteISLChannel> SatelliteNodeHelper::GetISLChannel() const
    {
        return m_channel;
    }


    Ptr<Node> SatelliteNodeHelper::Create() const
    {
        Ptr<Node> node = CreateObject<Node>();
        Install(node);

        //Ptr<SatelliteISLNetDevice> itf = m_itfFactory.CreateAndAggregate(node, m_channel);
        // itf->SetNode(node);
        // node->AddDevice(itf);

        return node;
    }


    void SatelliteNodeHelper::Install(Ptr<Node> node) const
    {
         Ptr<SatelliteISLNetDevice> itf = m_itfFactory.CreateAndAggregate(node, m_channel);

        //  Ptr<SatelliteNodeTag> tag = CreateObject<SatelliteNodeTag>();

        //  node->AggregateObject(tag);
    }


    void SatelliteNodeHelper::Install(NodeContainer nodes) const
    {
        for (NodeContainer::Iterator it = nodes.Begin(); it != nodes.End(); it++)
        {
            //Ptr<SatelliteISLNetDevice> itf = m_itfFactory.CreateAndAggregate(*it, m_channel);
            Install(*it);
        }
    }


    void SatelliteNodeHelper::printSatNodeInfo(std::ostream& out, const Ptr<Node> node)
    {
        Ptr<SatelliteISLNetDevice> itf = nullptr;
        if (node->GetNDevices() > 0)
        {
            itf = StaticCast<SatelliteISLNetDevice>(node->GetDevice(0));
        }

        Ptr<SatelliteNodeTag> tag = node->GetObject<SatelliteNodeTag>();

        if (tag != nullptr)
        {
            out << "> Tag: " << tag->GetTypeId().GetName() << "\n";
            out << "    Const.: " << int(tag->GetCID()) << "  Orbit: " << tag->GetOID() << "  Sat-ID: " << tag->GetId() << "\n";
            // out << "    OID: " << int(tag->GetOID()) << "\n";
            // out << "    CID: " << int(tag->GetCID()) << "\n";
        }
        else out << "No Satellite Node Tag found\n";

        if (itf != nullptr)
        {
            out << "> Terminals: " << itf->GetNTerminals() << "\n";
        }
        else out << "No Satellite ISL Interface found \n";


        out << "\n";
    }


    void SatelliteNodeHelper::SetInitIpStack(const bool onOff)
    {
        m_ipstack = onOff;
    }


    bool SatelliteNodeHelper::GetInitIpStack() const
    {
        return m_ipstack;
    }


}   /* namespace ns3 */