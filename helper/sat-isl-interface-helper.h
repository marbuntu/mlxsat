/**
 * @brief   Helper to setup Satellite ISL Interfaces
 * 
 * @file    sat-isl-interface-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-09-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SAT_ISL_INTERFACE_HELPER_H
#define SAT_ISL_INTERFACE_HELPER_H

#include "ns3/object.h"
#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/sat-isl-channel.h"
#include "ns3/sat-isl-terminal-helper.h"
#include "ns3/sat-isl-net-device.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"

namespace ns3
{


    class SatelliteISLInterfaceHelper : public Object
    {
    public:

        static TypeId GetTypeId();

        SatelliteISLInterfaceHelper();
        ~SatelliteISLInterfaceHelper();


        /**
         * @brief Create a new ISL Interface with Terminals and MAC48
         * 
         * @return Ptr<SatelliteISLNetDevice> 
         */
        Ptr<SatelliteISLNetDevice> Create() const;


        /**
         * @brief Create and Aggregate Interfaces for Nodes
         * 
         * @param nodes
         * @param channel 
         * @return NetDeviceContainer 
         */
        NetDeviceContainer Install(NodeContainer nodes, const Ptr<SatelliteISLChannel> channel) const;


        /**
         * @brief Create a And Aggregate Interface for a Node
         * 
         * @param node 
         * @param channel 
         * @return Ptr<SatelliteISLNetDevice> 
         */
        Ptr<SatelliteISLNetDevice> CreateAndAggregate(Ptr<Node> node, const Ptr<SatelliteISLChannel> channel) const;


        /**
         * @brief Add Terminal Template with Attributes
         * 
         * @tparam Args 
         * @param args 
         * @return Ptr<SatelliteISLTerminalHelper> 
         */
        template <typename... Args>
        Ptr<SatelliteISLTerminalHelper> AddTerminal(Args... args)
        {
            Ptr<SatelliteISLTerminalHelper> fac = CreateObjectWithAttributes<SatelliteISLTerminalHelper>(args...);
            m_terminalFactories.insert(m_terminalFactories.end(), fac);

            return fac;
        }


    private:

        /**
         * @brief Vector of Terminal Templates to create for each Node
         */
        std::vector<Ptr<SatelliteISLTerminalHelper>> m_terminalFactories;


    };  /* SatelliteISLInterfaceHelper */



    class DefaultISLInterfaceSetup
    {
    public:
        typedef enum {
            SYMMETRIC_4x = 0,
            ASYMMETRIC_2x2 = 1,
            ISOTROPIC = 2,
        } setup_t;


        static SatelliteISLInterfaceHelper GetDefaultFactory(setup_t setup);


    protected:
        DefaultISLInterfaceSetup() {};
        ~DefaultISLInterfaceSetup() {};

        /**
         * @brief 4 Terminals in N-S-E-W direction, Shared Antenna Characteristics
         * 
         * @return SatelliteISLInterfaceHelper 
         */
        static SatelliteISLInterfaceHelper _Symmetric4x();

        /**
         * @brief 2x2 Terminal Pairs in N-S and E-W direction. Pairs with shared Antenna Characteristics.
         * 
         * @return SatelliteISLInterfaceHelper 
         */
        static SatelliteISLInterfaceHelper _Asymmetric2x2();

        /**
         * @brief Single Terminal with Isotropic Radiation Characteristic
         * 
         * @return SatelliteISLInterfaceHelper 
         */
        static SatelliteISLInterfaceHelper _Isotropic();

    };  /* DefaultISLInterfaceSetup */




};  /* namespace ns3 */

#endif /* SAT_ISL_INTERFACE_HELPER_H */