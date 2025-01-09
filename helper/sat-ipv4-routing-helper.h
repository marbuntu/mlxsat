/**
 * @brief   Satellite IPv4 Routing Helper
 * 
 * @file    sat-ipv4-routing-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-11-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_IPv4_ROUTING_HELPER_H
#define SATELLITE_IPv4_ROUTING_HELPER_H


#include "ns3/ipv4-routing-helper.h"


namespace ns3
{

    class SatelliteIPv4RoutingHelper : public Ipv4RoutingHelper
    {
    public:

        SatelliteIPv4RoutingHelper();

        ~SatelliteIPv4RoutingHelper();


        SatelliteIPv4RoutingHelper* Copy() const;

        Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const;




    }; /* SatelliteIPv4RoutingHelper */



};  /* namespace ns3 */


#endif /* SATELLITE_IPv4_ROUTING_HELPER_H */