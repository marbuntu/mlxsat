/**
 * @brief   Satellite IPv4 Routing Helper
 * 
 * @file    sat-ipv4-routing-helper.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-11-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-ipv4-routing-helper.h"
#include "ns3/sat-isl-ipv4-routing.h"


namespace ns3
{

        SatelliteIPv4RoutingHelper::SatelliteIPv4RoutingHelper()
        {
        }


        SatelliteIPv4RoutingHelper::~SatelliteIPv4RoutingHelper()
        {
        }



        SatelliteIPv4RoutingHelper* SatelliteIPv4RoutingHelper::Copy() const
        {
            return new SatelliteIPv4RoutingHelper(*this);
        }



        Ptr<Ipv4RoutingProtocol> SatelliteIPv4RoutingHelper::Create(Ptr<Node> node) const
        {
            Ptr<SatelliteISLRoutingIPv4> rpc = CreateObject<SatelliteISLRoutingIPv4>();
            return rpc;
        }


}   /* namespace ns3 */