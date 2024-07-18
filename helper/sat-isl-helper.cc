/**
 * @brief   Inter-Satellite-Link Helper Class
 * 
 * @file    sat-isl-helper.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-helper.h"


namespace ns3
{

    SatelliteISLHelper::SatelliteISLHelper()
    {
        m_deviceFactory.SetTypeId("ns3::SatelliteISLNetDevice");
        m_channelFactory.SetTypeId("ns3::SatelliteStaticISLChannel");
        m_queueFactory.SetTypeId("ns3::DropTailQueue<Packet>");
        m_ptpMode = false;
    }


    NetDeviceContainer SatelliteISLHelper::Install(Ptr<Node> node, Ptr<SatelliteStaticISLChannel> channel) const
    {
        return NetDeviceContainer(InstallPriv(node, channel));
    }


    Ptr<NetDevice> SatelliteISLHelper::InstallPriv(Ptr<Node> node, Ptr<SatelliteStaticISLChannel> channel) const
    {
        Ptr<SatelliteISLNetDevice> device = m_deviceFactory.Create<SatelliteISLNetDevice>();
        return device;
    }


}   /* namespace ns3 */