/**
 * @brief   Utils to Setup the Simulation Examples
 * 
 * @file    demo-setup-helper.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-10-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#include "demo-setup-helper.h"



namespace ns3
{
    Ptr<WalkerConstellationHelper> SatDemoSetup::m_walker = nullptr;


    void SatDemoSetup::Initialize()
    {
        SatDemoSetup::m_walker = CreateObjectWithAttributes<WalkerConstellationHelper>(
            "Inclination", DoubleValue(66.0),
            "NumOfSats", IntegerValue(10),
            "NumOfOrbits", IntegerValue(10),
            "Altitude", DoubleValue(480)
        );

        m_walker->Initialize();
    }

    Ptr<WalkerConstellationHelper> SatDemoSetup::GetConstellation()
    {
        return SatDemoSetup::m_walker;
    }


    NodeContainer SatDemoSetup::GetDefaultNodes()
    {
        NodeContainer nodes;
        nodes.Add(m_walker->getSatellite(2 * 10 + 5)->GetObject<Node>());       // Sat 0
        nodes.Add(m_walker->getSatellite(2 * 10 + 6)->GetObject<Node>());       // Sat 1
        nodes.Add(m_walker->getSatellite(1 * 10 + 5)->GetObject<Node>());       // Sat 2
        nodes.Add(m_walker->getSatellite(2 * 10 + 4)->GetObject<Node>());       // Sat 3
        nodes.Add(m_walker->getSatellite(3 * 10 + 5)->GetObject<Node>());       // Sat 4

        return nodes;
    }




}   /* namespace ns3 */