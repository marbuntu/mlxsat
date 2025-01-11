/**
 * @brief   Utils to Setup the Simulation Examples
 * 
 * @file    demo-setup-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-10-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef DEMO_SETUP_HELPER_H
#define DEMO_SETUP_HELPER_H

#include <ns3/core-module.h>
#include <ns3/walker-constellation-helper.h>
#include <ns3/node-container.h>
#include <ns3/sat-isl-channel.h>


namespace ns3
{

    /**
     * @category Simulation Helper
     * @brief Helper Class to Setup a Simulation based on the Paper:
     * 
     *      On-Board Federated Learning for Satellite Clusters with Inter-Satellite Links
     *      (Ref.: arXiv:2307.08346v2  [cs.IT]  21 Dec 2023)
     * 
     *  ### Constellations:
     * 
     *      + Walker-Delta: 60°: 40/5/1
     * 
     *      + Walker-Start: 85°: 40/5/1
     * 
     */
    class SatDemoSatClusterForFL
    {
    public:
        static void Initialize();



    protected:

        SatDemoSatClusterForFL();
        ~SatDemoSatClusterForFL();
    
        std::vector<Ptr<SatSGP4MobilityModel>> m_walkerDelta;
        std::vector<Ptr<SatSGP4MobilityModel>> m_walkerStar;
    

    };  /* SatDemoSatClusterForFL*/



    class SatDemoSetup
    {
    public:

        static void Initialize();


        static Ptr<WalkerConstellationHelper> GetConstellation();


        static NodeContainer GetDefaultNodes();


        static Ptr<SatelliteISLChannel> GetDefaultChannel();


    protected:
        SatDemoSetup(){};
        ~SatDemoSetup(){};

        static Ptr<WalkerConstellationHelper> m_walker;
        


    }; /* SatDemoSetup */


}   /* namespace ns3 */

#endif /* DEMO_SETUP_HELPER_H */