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


#ifndef SAT_ISL_IPv4_ROUTING_H
#define SAT_ISL_IPv4_ROUTING_H


#include <ns3/object.h>
#include <ns3/ipv4.h>
#include <ns3/ipv4-routing-protocol.h>
#include <ns3/ipv4-l3-protocol.h>


namespace ns3
{

    typedef Callback<void, bool, const Ipv4Route&, Ptr<Packet>, const Ipv4Header&> RouteReplyCallback;


    class SatelliteISLRoutingIPv4 : public Ipv4RoutingProtocol
    {
    public:

        static TypeId GetTypeId();


        SatelliteISLRoutingIPv4();
        ~SatelliteISLRoutingIPv4();


        bool RequestRoute (uint32_t ifIndex,
                                const Ipv4Header &ipHeader,
                                Ptr<Packet> packet,
                                RouteReplyCallback routeReply);


        /**
         * \brief Query routing cache for an existing route, for an outbound packet
         *
         * This lookup is used by transport protocols.  It does not cause any
         * packet to be forwarded, and is synchronous.  Can be used for
         * multicast or unicast.  The Linux equivalent is ip_route_output()
         *
         * The header input parameter may have an uninitialized value
         * for the source address, but the destination address should always be
         * properly set by the caller.
         *
         * \param p packet to be routed.  Note that this method may modify the packet.
         *          Callers may also pass in a null pointer.
         * \param header input parameter (used to form key to search for the route)
         * \param oif Output interface Netdevice.  May be zero, or may be bound via
         *            socket options to a particular output interface.
         * \param sockerr Output parameter; socket errno
         *
         * \returns a code that indicates what happened in the lookup
         */
        Ptr<Ipv4Route> RouteOutput(Ptr<Packet> p,
                                        const Ipv4Header& header,
                                        Ptr<NetDevice> oif,
                                        Socket::SocketErrno& sockerr);

        /**
         * \brief Route an input packet (to be forwarded or locally delivered)
         *
         * This lookup is used in the forwarding process.  The packet is
         * handed over to the Ipv4RoutingProtocol, and will get forwarded onward
         * by one of the callbacks.  The Linux equivalent is ip_route_input().
         * There are four valid outcomes, and a matching callbacks to handle each.
         *
         * \param p received packet
         * \param header input parameter used to form a search key for a route
         * \param idev Pointer to ingress network device
         * \param ucb Callback for the case in which the packet is to be forwarded
         *            as unicast
         * \param mcb Callback for the case in which the packet is to be forwarded
         *            as multicast
         * \param lcb Callback for the case in which the packet is to be locally
         *            delivered
         * \param ecb Callback to call if there is an error in forwarding
         * \returns true if the Ipv4RoutingProtocol takes responsibility for
         *          forwarding or delivering the packet, false otherwise
         */
        bool RouteInput(Ptr<const Packet> p,
                                const Ipv4Header& header,
                                Ptr<const NetDevice> idev,
                                UnicastForwardCallback ucb,
                                MulticastForwardCallback mcb,
                                LocalDeliverCallback lcb,
                                ErrorCallback ecb);

        /**
         * \param interface the index of the interface we are being notified about
         *
         * Protocols are expected to implement this method to be notified of the state change of
         * an interface in a node.
         */
        void NotifyInterfaceUp(uint32_t interface);

        /**
         * \param interface the index of the interface we are being notified about
         *
         * Protocols are expected to implement this method to be notified of the state change of
         * an interface in a node.
         */
        void NotifyInterfaceDown(uint32_t interface);

        /**
         * \param interface the index of the interface we are being notified about
         * \param address a new address being added to an interface
         *
         * Protocols are expected to implement this method to be notified whenever
         * a new address is added to an interface. Typically used to add a 'network route' on an
         * interface. Can be invoked on an up or down interface.
         */
        void NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address);

        /**
         * \param interface the index of the interface we are being notified about
         * \param address a new address being added to an interface
         *
         * Protocols are expected to implement this method to be notified whenever
         * a new address is removed from an interface. Typically used to remove the 'network route' of
         * an interface. Can be invoked on an up or down interface.
         */
        void NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address);

        /**
         * \param ipv4 the ipv4 object this routing protocol is being associated with
         *
         * Typically, invoked directly or indirectly from ns3::Ipv4::SetRoutingProtocol
         */
        void SetIpv4(Ptr<Ipv4> ipv4);

        /**
         * \brief Print the Routing Table entries
         *
         * \param stream The ostream the Routing table is printed to
         * \param unit The time unit to be used in the report
         */
        void PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;


        void AddRoutingEntry(Ipv4Address network, Ipv4Mask netmask, Ipv4Address nexthop, uint32_t interface);

        typedef std::list<std::pair<Ipv4RoutingTableEntry*, uint32_t>> NetworkRoutes;

    protected:

        Ptr<Ipv4> m_ipv4;

        NetworkRoutes m_routes;

        Ptr<Ipv4Route> LookUp(Ipv4Address dst, Ptr<NetDevice> oif = nullptr);


    private:

    }; /* SatelliteISLRoutingIPv4 */


};  /* namespace ns3 */


#endif /* SAT_ISL_IPv4_ROUTING_H */