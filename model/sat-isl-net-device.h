/**
 * @brief 
 * 
 * @file    sat-isl-net-device.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 0.1
 * @date    2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#ifndef SAT_ISL_NET_DEVICE_H
#define SAT_ISL_NET_DEVICE_H


#include "ns3/mac48-address.h"

#include "ns3/data-rate.h"
#include "ns3/event-id.h"
#include "ns3/net-device.h"
#include "ns3/queue-fwd.h"
#include "ns3/traced-callback.h"

#include "ns3/sat-isl-terminal.h"

#include <stdint.h>
#include <string>

namespace ns3
{

class SatelliteISLChannel;
class SatelliteISLTerminal;
class ErrorModel;



/**
 * \ingroup netdevice
 * 
 * @brief   NetDevice for Inter-Satellite-Links
 */
class SatelliteISLNetDevice : public NetDevice
{
public:

    static TypeId GetTypeId();
    SatelliteISLNetDevice();


        /**
     * Receive a packet from a connected SimpleChannel.  The
     * SimpleNetDevice receives packets from its connected channel
     * and then forwards them by calling its rx callback method
     *
     * \param packet Packet received on the channel
     * \param protocol protocol number
     * \param to address packet should be sent to
     * \param from address packet was sent from
     */
    void Receive(Ptr<Packet> packet, uint16_t protocol, Mac48Address to, Mac48Address from);


    /**
     * Attach a channel to this net device.  This will be the
     * channel the net device sends on
     *
     * \param channel channel to assign to this net device
     *
     */
    void SetChannel(Ptr<SatelliteISLChannel> channel);


    /**
     * Attach a queue to the SimpleNetDevice.
     *
     * \param queue Ptr to the new queue.
     */
    void SetQueue(Ptr<Queue<Packet>> queue);


    /**
     * Get a copy of the attached Queue.
     *
     * \returns Ptr to the queue.
     */
    Ptr<Queue<Packet>> GetQueue() const;


    /**
     * Attach a receive ErrorModel to the SimpleNetDevice.
     *
     * The SimpleNetDevice may optionally include an ErrorModel in
     * the packet receive chain.
     *
     * \see ErrorModel
     * \param em Ptr to the ErrorModel.
     */
    void SetReceiveErrorModel(Ptr<ErrorModel> em);


    Mac48Address GetMacAddress() const;


    /**
     * @brief Register new Terminal to this NetDevice
     * 
     * @param terminal  New ISL Terminal
     */
    void RegisterISLTerminal(const Ptr<SatelliteISLTerminal> terminal);


    /**
     * @brief Get ISL Terminal with index id
     * 
     * @param id 
     * @return Ptr<SatelliteISLTerminal> 
     */
    Ptr<SatelliteISLTerminal> GetISLTerminal(const size_t id) const;


    /**
     * @brief Overwrite Local Reference
     * 
     * @param ref 
     */
    void SetLocalReference(const Ptr<LVLHReference> ref);


    /**
     * @brief Get Local Reference
     * 
     * @return Ptr<LVLHReference> 
     */
    Ptr<LVLHReference> GetLocalReference() const;


    bool EnqueuePacket(Ptr<Packet> pck, Mac48Address src, Mac48Address dst, uint16_t proto);


    bool EnqueueBroadcast(Ptr<Packet> pck, Mac48Address src, Mac48Address dst, uint16_t proto);


    void SetMinDR(DataRate minDR);
    DataRate GetMinDR() const;

    void SetRxSensitivity(const double rxsens_dbm);
    double GetRxSensitivity() const;


    // inherited from NetDevice base class.
    void SetIfIndex(const uint32_t index) override;
    uint32_t GetIfIndex() const override;
    Ptr<Channel> GetChannel() const override;
    void SetAddress(Address address) override;
    Address GetAddress() const override;
    bool SetMtu(const uint16_t mtu) override;
    uint16_t GetMtu() const override;
    bool IsLinkUp() const override;
    void AddLinkChangeCallback(Callback<void> callback) override;
    bool IsBroadcast() const override;
    Address GetBroadcast() const override;
    bool IsMulticast() const override;
    Address GetMulticast(Ipv4Address multicastGroup) const override;
    bool IsPointToPoint() const override;
    bool IsBridge() const override;
    bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) override;
    bool SendFrom(Ptr<Packet> packet,
                  const Address& source,
                  const Address& dest,
                  uint16_t protocolNumber) override;
    Ptr<Node> GetNode() const override;
    void SetNode(Ptr<Node> node) override;
    bool NeedsArp() const override;
    void SetReceiveCallback(NetDevice::ReceiveCallback cb) override;

    Address GetMulticast(Ipv6Address addr) const override;

    void SetPromiscReceiveCallback(PromiscReceiveCallback cb) override;
    bool SupportsSendFrom() const override;


    size_t GetNTerminals() const;


    /**
     * @brief Get Iterator, pointing to the first Terminal ( id = 0 )
     * 
     * @return std::vector<Ptr<SatelliteISLTerminal>>::iterator 
     */
    std::vector<Ptr<SatelliteISLTerminal>>::iterator BeginTerminals()
    {
        return m_terminals.begin();
    }


    /**
     * @brief Get Iterator pointing to the last Terminal ( id = N )
     * 
     * @return std::vector<Ptr<SatelliteISLTerminal>>::iterator 
     */
    std::vector<Ptr<SatelliteISLTerminal>>::iterator EndTerminals()
    {
        return m_terminals.end();
    }


protected:
    void DoDispose() override;




private:

    /**
     * The StartTransmission method is used internally to start the process
     * of sending a packet out on the channel, by scheduling the
     * FinishTransmission method at a time corresponding to the transmission
     * delay of the packet.
     */
    void StartTransmission();

    /**
     * The FinishTransmission method is used internally to finish the process
     * of sending a packet out on the channel.
     * \param packet The packet to send on the channel
     */
    void FinishTransmission(Ptr<Packet> packet);


    //LVLHReference m_reflocal;

    Ptr<SatelliteISLChannel> m_channel;
    Ptr<Node> m_node;
    Mac48Address m_address;
    Ptr<ErrorModel> m_recErrModel;
    double m_atpDelay;

    NetDevice::ReceiveCallback m_rxCallback;                //!< Receive callback
    NetDevice::PromiscReceiveCallback m_promiscCallback;    //!< Promiscuous receive callback
    
    uint16_t m_mtu;                             //!< MTU
    uint32_t m_ifIndex;                         //!< Interface index
    Ptr<Queue<Packet>> m_queue;                 //!< The Queue for outgoing packets.
    //Ptr<Queue<SatelliteISLSignal>> m_queue;

    bool m_linkUp;                              //!< Indicator - link up / down
    EventId m_finishTransmissionEvent;            //!< the Tx Complete event


    /**
     * List of callbacks for link state changes (up or down).
     */
    TracedCallback<> m_linkChangeCallbacks;

    
    /**
     * Flag indicating whether or not the NetDevice is a Point to Point model.
     * Enabling this will disable Broadcast and Arp.
     */
    bool m_pointToPointMode;


    DataRate m_minDR;

    double m_rxsensdbm;

    
    /**
     * @brief Use Global Interconnect Matrix for Known Neighbours
     */
    bool m_useICM;


    /**
     * The trace source fired when the phy layer drops a packet it has received
     * due to the error model being active.  Although SimpleNetDevice doesn't
     * really have a Phy model, we choose this trace source name for alignment
     * with other trace sources.
     *
     * \see class CallBackTraceSource
     */
    TracedCallback<Ptr<const Packet>> m_phyRxDropTrace;


    TracedCallback<Ptr<const Packet>> m_phyRxTrace;
    TracedCallback<Ptr<const Packet>, const Address&> m_phyTxTrace;



    Ptr<LVLHReference> m_refLVLH;


    std::vector <Ptr<SatelliteISLTerminal>> m_terminals;

};



}   /* namespace ns3 */

#endif  /* SAT_ISL_NET_DEVICE_H */