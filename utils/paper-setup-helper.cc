/**
 * @brief   Simulation Setup for Globecom Paper
 * 
 * @file    paper-setup-helper.c
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2025-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "paper-setup-helper.h"
#include <ns3/walker-constellation-helper.h>


namespace ns3
{
    std::vector<Ptr<WalkerConstellationHelper>> GlobecomSetupHelper::m_constellations;
    std::vector<GlobecomSetupHelper::constellationSetup> GlobecomSetupHelper::m_setups;

    GlobecomSetupHelper::GlobecomSetupHelper()
    {
        // // IDs Range from 1 to N+1
        // for (size_t n = 0; n < 3; n++)
        // {
    
            
        //     Ptr<WalkerConstellationHelper> constellation = CreateObjectWithAttributes<WalkerConstellationHelper>(
        //         "WalkerType", EnumValue(WalkerConstellationHelper::WALKER_DELTA),
        //         "Inclination", DoubleValue(60.0),
        //         "SatsPerOrbit", IntegerValue(8),
        //         "NumOfOrbits", IntegerValue(5),
        //         "Phasing", DoubleValue(9.0),
        //         "Altitude", DoubleValue(2000),
        //         "ConstellationID", IntegerValue(n)
        //     );
            
        //     m_constellations.push_back(constellation);
        // }

    }

    void GlobecomSetupHelper::LoadDefault()
    {

        for (size_t n = 0; n < 3; n++)
        {
            constellationSetup setup;
            
            // Create Constellation Helper
            setup.CID = n + 1;
            setup.constellation_setup = CreateObjectWithAttributes<WalkerConstellationHelper>(
                "WalkerType",   EnumValue(WalkerConstellationHelper::WALKER_DELTA),
                "Inclination",  DoubleValue(60.0),
                "SatsPerOrbit", IntegerValue(8),
                "NumOfOrbits",  IntegerValue(5),
                "Phasing",      DoubleValue(9.0),
                "Altitude",     DoubleValue(2000),
                "ConstellationID", IntegerValue(setup.CID)
            );

            // Set Interface Factory
            setup.interface_setup = DefaultISLInterfaceSetup::GetDefaultFactory((DefaultISLInterfaceSetup::setup_t) n);

            // Create Channel
            setup.channel_setup = CreateObject<SatelliteISLChannel>();
            Ptr<FriisPropagationLossModel> loss = CreateObject<FriisPropagationLossModel>();
            Ptr<ConstantSpeedPropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel>();
            setup.channel_setup->SetPropagationLossModel(loss);
            setup.channel_setup->SetPropagationDelayModel(delay);


            m_setups.push_back(setup);
        }

    }


    void GlobecomSetupHelper::Initialize()
    {

        for (auto setup : m_setups)
        {
            setup.constellation_setup->Initialize();   

            NodeContainer nodes;
            nodes.Add(setup.constellation_setup->getSatellite(2 * 8 + 5)->GetObject<Node>());       // Sat 0
            nodes.Add(setup.constellation_setup->getSatellite(2 * 8 + 6)->GetObject<Node>());       // Sat 1
            nodes.Add(setup.constellation_setup->getSatellite(1 * 8 + 5)->GetObject<Node>());       // Sat 2
            nodes.Add(setup.constellation_setup->getSatellite(2 * 8 + 4)->GetObject<Node>());       // Sat 3
            nodes.Add(setup.constellation_setup->getSatellite(3 * 8 + 5)->GetObject<Node>());       // Sat 4

            setup.interface_setup.Install(nodes, setup.channel_setup);
        }
    }




}   /* namespace ns3 */