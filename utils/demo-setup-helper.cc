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

    Ptr<WalkerConstellationHelper> SatDemoSatClusterForFL::m_constellation = nullptr;

    void SatDemoSatClusterForFL::Initialize()
    {

        m_constellation = CreateObjectWithAttributes<WalkerConstellationHelper>(
            "WalkerType", EnumValue(WalkerConstellationHelper::WALKER_DELTA),
            "Inclination", DoubleValue(60.0),
            "SatsPerOrbit", IntegerValue(8),
            "NumOfOrbits", IntegerValue(5),
            "Phasing", DoubleValue(9.0),
            "Altitude", DoubleValue(2000),
            "ConstellationID", IntegerValue(3)
        );
        m_constellation->Initialize();

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


    NodeContainer SatDemoSatClusterForFL::GetDefaultNodes()
    {
        NodeContainer nodes;
        nodes.Add(m_constellation->getSatellite(2 * 8 + 5)->GetObject<Node>());       // Sat 0
        nodes.Add(m_constellation->getSatellite(2 * 8 + 6)->GetObject<Node>());       // Sat 1
        nodes.Add(m_constellation->getSatellite(1 * 8 + 5)->GetObject<Node>());       // Sat 2
        nodes.Add(m_constellation->getSatellite(2 * 8 + 4)->GetObject<Node>());       // Sat 3
        nodes.Add(m_constellation->getSatellite(3 * 8 + 5)->GetObject<Node>());       // Sat 4

        return nodes;
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


    Ptr<SatelliteISLChannel> SatDemoSetup::GetDefaultChannel()
    {

        Ptr<SatelliteISLChannel> channel = CreateObject<SatelliteISLChannel>();
        Ptr<FriisPropagationLossModel> loss_model = CreateObject<FriisPropagationLossModel>();
        Ptr<ConstantSpeedPropagationDelayModel> delay_model = CreateObject<ConstantSpeedPropagationDelayModel>();
        channel->SetPropagationLossModel(loss_model);
        channel->SetPropagationDelayModel(delay_model);

        return channel;
    }

}   /* namespace ns3 */