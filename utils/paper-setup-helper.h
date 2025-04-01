/**
 * @brief   Simulation Setup for Globecom Paper
 * 
 * @file    paper-setup-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2025-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef PAPER_SETUP_HELPER_H
#define PAPER_SETUP_HELPER_H


#include <ns3/core-module.h>
#include <ns3/walker-constellation-helper.h>
#include <ns3/node-container.h>
#include <ns3/sat-isl-channel.h>
#include <ns3/sat-isl-interface-helper.h>


namespace ns3
{
    /**
     *  ## Globecom Paper Simulation Setup Helper
     *  
     * 
     *  ### Constellation:
     * 
     *      + Walker-Delta: 60°: 40/5/1
     * 
     *  ### Satellite Setups:
     * 
     *      #### I. Isotropical Setup
     *      + Fixed Gain 40 dBm
     *      + Isotropical Radiation Pattern (no)
     * 
     *      #### II. Symmetrical Setup
     *      + 4 Terminals with equal properties
     *      + Spaced by 90° around z-Axis (in x-y Plane)
     *      + Max. Gain 40 dBm
     *      
     *      #### III. Asymmetrical Setup
     * 
     */
    class GlobecomSetupHelper
    {
    public:

        typedef struct 
        {
            uint8_t CID;    // Constellation ID 
            SatelliteISLInterfaceHelper interface_setup;  // Interface Setup Helper
            Ptr<WalkerConstellationHelper> constellation_setup;
            Ptr<SatelliteISLChannel> channel_setup;
        } constellationSetup;


        static void LoadDefault();

        static void Initialize();

        static Ptr<WalkerConstellationHelper> GetConstellation();

        static NodeContainer GetDefaultNodes(size_t cid);

        static Ptr<SatelliteISLChannel> GetDefaultChannel();


    protected:

        GlobecomSetupHelper();
        ~GlobecomSetupHelper();
        

        // Number of Constellations (Scenarios) in the Simulation        
        static std::vector<constellationSetup> m_setups;



        // Reference to Constellation Helpers
        static std::vector<Ptr<WalkerConstellationHelper>> m_constellations;
        // static std::vector<int> m_terminalHelpers;

    };

};  /* namespace ns3 */


#endif /* PAPER_SETUP_HELPER_H */