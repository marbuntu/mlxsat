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


#include "ns3/walker-constellation-helper.h"
#include "demo-setup-helper.h"



namespace ns3
{

    void SatDemoSatClusterForFL::Initialize()
    {

        Ptr<WalkerConstellationHelper> test = CreateObjectWithAttributes<WalkerConstellationHelper>(
            "WalkerType", EnumValue(WalkerConstellationHelper::WALKER_STAR),
            "Inclination", DoubleValue(60.0),
            "SatsPerOrbit", IntegerValue(40),
            "NumOfOrbits", IntegerValue(5),
            "Phasing", DoubleValue(9.0),
            "Altitude", DoubleValue(2000),
            "ConstellationID", IntegerValue(3)
        );
        test->Initialize();

        // Ptr<WalkerConstellationHelper> delta = CreateObjectWithAttributes<WalkerConstellationHelper>(
        //     "WalkerType", EnumValue(WalkerConstellationHelper::WALKER_DELTA),
        //     "Inclination", DoubleValue(60.0),
        //     "SatsPerOrbit", IntegerValue(40),
        //     "NumOfOrbits", IntegerValue(5),
        //     "Phasing", DoubleValue(9.0),
        //     "Altitude", DoubleValue(2000),
        //     "ConstellationID", IntegerValue(1)
        // );

        // delta->Initialize();
        // delta->LogInitialPositions("./pyplot/data/delta", ".txt");

        // Ptr<WalkerConstellationHelper> star = CreateObjectWithAttributes<WalkerConstellationHelper>(
        //     "WalkerType", EnumValue(WalkerConstellationHelper::WALKER_STAR),
        //     "Inclination", DoubleValue(85.0),
        //     "SatsPerOrbit", IntegerValue(40),
        //     "NumOfOrbits", IntegerValue(5),
        //     "Phasing", DoubleValue(9.0),
        //     "Altitude", DoubleValue(2000),
        //     "ConstellationID", IntegerValue(2)
        // );

        // star->Initialize();
        // star->LogInitialPositions("./pyplot/data/star", ".txt");

    }






    Ptr<WalkerConstellationHelper> SatDemoSetup::m_walker = nullptr;


    void SatDemoSetup::Initialize()
    {
        SatDemoSetup::m_walker = CreateObjectWithAttributes<WalkerConstellationHelper>(
            "WalkerType", EnumValue(WalkerConstellationHelper::WALKER_STAR),
            "Inclination", DoubleValue(66.0),
            "SatsPerOrbit", IntegerValue(10),
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