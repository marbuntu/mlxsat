/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    _template.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-ipv4-routing.h"
#include "ns3/ipv4-routing-table-entry.h"


namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("SatISLRoutingIPv4");


    TypeId SatelliteISLRoutingIPv4::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLRoutingIPv4")
            .SetParent<Ipv4RoutingProtocol>()
            .AddConstructor<SatelliteISLRoutingIPv4>()
            .SetGroupName("Internet")
        ;

        return tid;
    }


    SatelliteISLRoutingIPv4::SatelliteISLRoutingIPv4()
    {
    }


    SatelliteISLRoutingIPv4::~SatelliteISLRoutingIPv4()
    {
    }


    bool SatelliteISLRoutingIPv4::RequestRoute (uint32_t ifIndex,
                             const Ipv4Header &ipHeader,
                             Ptr<Packet> packet,
                             RouteReplyCallback routeReply)
    {
        NS_LOG_FUNCTION(this << ifIndex << ipHeader << packet);
        return false;
    }

    Ptr<Ipv4Route> SatelliteISLRoutingIPv4::RouteOutput(Ptr<Packet> p,
                                        const Ipv4Header& header,
                                        Ptr<NetDevice> oif,
                                        Socket::SocketErrno& sockerr)
    {
        Ipv4Address src = header.GetSource();
        Ipv4Address dst = header.GetDestination();
        oif = m_ipv4->GetNetDevice(1);

        NS_LOG_FUNCTION(this << p << src << " to " << dst << oif);

        // static Ipv4Route rt;
        // rt.SetDestination(dst);
        // rt.SetSource(src);
        // rt.SetOutputDevice(oif);
        // rt.SetGateway(Ipv4Address("10.1.1.2"));

        Ptr<Ipv4Route> route = nullptr;

        for (auto it = m_routes.begin(); it != m_routes.end(); it++)
        {
            Ipv4RoutingTableEntry* entry = it->first;

            if (entry->GetDest() == dst)
            {
                route = Create<Ipv4Route>();
                route->SetDestination(dst);
                route->SetSource(src);
                route->SetGateway(entry->GetGateway());
                route->SetOutputDevice(
                    m_ipv4->GetNetDevice(entry->GetInterface())
                );
            }
        }



        return route;
    }



    bool SatelliteISLRoutingIPv4::RouteInput(Ptr<const Packet> p,
                                const Ipv4Header& header,
                                Ptr<const NetDevice> idev,
                                UnicastForwardCallback ucb,
                                MulticastForwardCallback mcb,
                                LocalDeliverCallback lcb,
                                ErrorCallback ecb)
    {
        NS_LOG_FUNCTION(this << p << header << idev );

        uint32_t iif = m_ipv4->GetInterfaceForDevice(idev);

        if (header.GetDestination().IsMulticast())
        {
            NS_LOG_FUNCTION(this << "Is Multicaste - Not Implemented!");
        }

        if (m_ipv4->IsDestinationAddress(header.GetDestination(), iif))
        {
            NS_LOG_FUNCTION("Local Delivery to " << header.GetDestination());
            lcb(p, header, iif);
            return true;
        }

        if (m_ipv4->IsForwarding(iif) == false)
        {
            NS_LOG_FUNCTION(this << "Forwarding not Implemented");
            ecb(p, header, Socket::ERROR_NOROUTETOHOST);
            return true;
        }

        Ptr<Ipv4Route> route = LookUp(header.GetDestination());
        if (route)
        {
            NS_LOG_FUNCTION(this << "Unicast Fwd!");
            ucb(route, p, header);
            return true;
        }

        return false;
    }


    void SatelliteISLRoutingIPv4::NotifyInterfaceUp(uint32_t interface)
    {
        NS_LOG_FUNCTION(this);
    }


    void SatelliteISLRoutingIPv4::NotifyInterfaceDown(uint32_t interface)
    {
        NS_LOG_FUNCTION(this);
    }


    void SatelliteISLRoutingIPv4::NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address)
    {
        NS_LOG_FUNCTION(this);
    }


    void SatelliteISLRoutingIPv4::NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address)
    {
        NS_LOG_FUNCTION(this);
    }


    void SatelliteISLRoutingIPv4::SetIpv4(Ptr<Ipv4> ipv4)
    {
        NS_LOG_FUNCTION(this);
        m_ipv4 = ipv4;
    }


    void SatelliteISLRoutingIPv4::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
        NS_LOG_FUNCTION(this);
    }


    void SatelliteISLRoutingIPv4::AddRoutingEntry(Ipv4Address network, Ipv4Mask netmask, Ipv4Address nexthop, uint32_t interface)
    {
        Ipv4RoutingTableEntry route = 
            Ipv4RoutingTableEntry::CreateNetworkRouteTo(network, netmask, nexthop, interface);
        
        Ipv4RoutingTableEntry* route_p = new Ipv4RoutingTableEntry(route);
        m_routes.emplace_back(route_p, 5);

    }

    Ptr<Ipv4Route> SatelliteISLRoutingIPv4::LookUp(Ipv4Address dst, Ptr<NetDevice> oif)
    {
        Ptr<Ipv4Route> route = nullptr;

        if (dst.IsLocalMulticast())
        {
            NS_LOG_FUNCTION(this << "Multicast not implemented!");
        }

        for (auto it = m_routes.begin(); it != m_routes.end(); it++)
        {
            Ipv4Address entry = (*it->first).GetDestNetwork();
            Ipv4Mask mask = (*it->first).GetDestNetworkMask();

            if (mask.IsMatch(dst, entry))
            {
                NS_LOG_FUNCTION(this << "Found a match!");
                
                Ipv4RoutingTableEntry *tble = (it->first);
                route = Create<Ipv4Route>();
                route->SetDestination(tble->GetDest());
                route->SetGateway(tble->GetGateway());

                uint32_t itfn = tble->GetInterface();
                Ipv4Address saddr = m_ipv4->SourceAddressSelection(itfn, tble->GetDest());
                route->SetSource(saddr); //m_ipv4->SourceAddressSelection(tble->GetInterface(), tble->GetDest()));
                route->SetOutputDevice(m_ipv4->GetNetDevice(tble->GetInterface()));
            }
        }

        return route;
    }


}   /* namespace ns3 */