/**
 * @brief   Inter-Satellite-Link Helper Class
 * 
 * @file    sat-isl-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_ISL_HELPER
#define SATELLITE_ISL_HELPER


#include "ns3/sat-static-isl-channel.h"

#include "ns3/net-device-container.h"
#include "ns3/node.h"
#include "ns3/object-factory.h"



namespace ns3
{



class SatelliteISLTerminal
{

}; /* SatelliteISLTerminal */


/**
 * @brief 
 */
class SatelliteISLHelper
{
public:

    SatelliteISLHelper();

    virtual ~SatelliteISLHelper() {};


    NetDeviceContainer Install(Ptr<Node> node) const;

    NetDeviceContainer Install(Ptr<Node> node, Ptr<SatelliteStaticISLChannel> channel) const;


private:

    /**
     * This method creates an ns3::SimpleNetDevice with the attributes configured by
     * SimpleNetDeviceHelper::SetDeviceAttribute and then adds the device to the node and
     * attaches the provided channel to the device.
     *
     * \param node The node to install the device in
     * \param channel The channel to attach to the device.
     * \returns The new net device.
     */
    Ptr<NetDevice> InstallPriv(Ptr<Node> node, Ptr<SatelliteStaticISLChannel> channel) const;


    ObjectFactory m_queueFactory;       //!< Transmission Queue Factory
    ObjectFactory m_deviceFactory;      //!< ISL NetDevice Factory
    ObjectFactory m_channelFactory;     //!< Channel Factory

    bool m_ptpMode;                     //!< PointToPoint Mode ON/OFF

}; /* SatelliteISLHelper */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_HELPER */