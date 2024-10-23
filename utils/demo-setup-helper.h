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

namespace ns3
{

    class SatDemoSetup
    {
    public:

        static void Initialize();


        static Ptr<WalkerConstellationHelper> GetConstellation();


        static NodeContainer GetDefaultNodes();



    protected:
        SatDemoSetup(){};
        ~SatDemoSetup(){};

        static Ptr<WalkerConstellationHelper> m_walker;
        


    }; /* SatDemoSetup */


}   /* namespace ns3 */

#endif /* DEMO_SETUP_HELPER_H */