/**
 * @file    sat-net-device.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @brief   Static Inter Satellite Net-Device 
 * @version 0.1
 * @date    2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#pragma once


#include "ns3/net-device.h"
#include "ns3/mac48-address.h"

namespace ns3
{

/**
 * \ingroup netdevice
 * 
 * \brief Abstract Base Class for Satellite Net Device
 */
class SatelliteNetDevice : public NetDevice
{
public:

    /**
     * @brief Get the Type ID
     * 
     * @return Object TypeId 
     */
    static TypeId GetTypeId();

    SatelliteNetDevice();


    void Receive(Ptr<Packet> packet, uint16_t protocol, Mac48Address dst, Mac48Address src);

    void SetChannel(Ptr<Channel> channel);


private:

};

SatelliteNetDevice::SatelliteNetDevice(/* args */)
{
}

SatelliteNetDevice::~SatelliteNetDevice()
{
}



}   /* namespace ns3 */