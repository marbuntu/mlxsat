/**
 * @brief   Satellite Node Helper
 * 
 * @file    sat-node-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-10-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_NODE_HELPER_H
#define SATELLITE_NODE_HELPER_H


#include "ns3/object.h"
#include "ns3/node.h"

#include "sat-isl-interface-helper.h"


namespace ns3
{

    class SatelliteNodeHelper : public Object
    {
    public:

        static TypeId GetTypeId();

        SatelliteNodeHelper();
        ~SatelliteNodeHelper();

        void SetInterfaceAttribute(std::string attr, const AttributeValue &value);

        Ptr<SatelliteISLInterfaceHelper> GetInterfaceFactory();

        
        void SetISLChannel(const Ptr<SatelliteISLChannel> channel);

        Ptr<SatelliteISLChannel> GetISLChannel() const;


        Ptr<Node> Create() const;

        Ptr<Node> Create(size_t N) const;

        void Install(Ptr<Node> node) const;

        void Install(NodeContainer nodes) const;

        void printSatNodeInfo(std::ostream& out, const Ptr<Node> node);


        friend std::ostream& operator<<(std::ostream& out, const Node &node);


        void SetInitIpStack(const bool onOff);
        bool GetInitIpStack() const;



    protected:
        SatelliteISLInterfaceHelper m_itfFactory;
        Ptr<SatelliteISLChannel> m_channel;

        bool m_ipstack;

    }; /* SatelliteNodeHelper */


};  /* namespace ns3 */


#endif /* SATELLITE_NODE_HELPER_H */