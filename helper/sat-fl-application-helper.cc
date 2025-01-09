/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    sat-fl-application-helper.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-fl-application-helper.h"


namespace ns3
{

    SatFLApplicationHelper::SatFLApplicationHelper()
    {
        m_factory.SetTypeId(SatelliteFLApplication::GetTypeId());
    }


    void SatFLApplicationHelper::SetAttribute(std::string name, const AttributeValue &value)
    {
        m_factory.Set(name, value);
    }


    ApplicationContainer SatFLApplicationHelper::Install(NodeContainer nodes)
    {
        ApplicationContainer apps;
        for (auto it = nodes.Begin(); it != nodes.End(); it++)
        {
            Ptr<Node> node = *it;
            Ptr<SatelliteFLApplication> flapp = m_factory.Create<SatelliteFLApplication>();
            node->AddApplication(flapp);
            apps.Add(flapp);
        }

        return apps;
    }


}   /* namespace ns3 */