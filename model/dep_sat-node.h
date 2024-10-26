/**
 * @brief   HEADER TEMPLATE
 * 
 * @file    _template.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_NODE_H
#define SATELLITE_NODE_H

#include "ns3/object.h"
#include "ns3/node.h"
#include "ns3/sat-isl-terminal.h"
#include "ns3/sat-isl-net-device.h"


namespace ns3
{

    class SatelliteNodeHelper : public Object
    {
    public:
        static TypeId GetTypeId();
        TypeId GetInstanceTypeId() const;

        SatelliteNodeHelper();

        ~SatelliteNodeHelper();
        

        size_t AddISLTerminal(const std::string uid, Ptr<SatelliteISLTerminal> terminal);


    protected:
    private:

        // Node m_node;
        Ptr<Node> m_node;
        Ptr<SatelliteISLNetDevice> m_netdevice; 
        std::unordered_map<std::string, Ptr<SatelliteISLTerminal>> m_terminals;
    

    }; /* SatelliteNode */

};  /* namespace ns3 */


#endif /* SATELLITE_NODE_H */