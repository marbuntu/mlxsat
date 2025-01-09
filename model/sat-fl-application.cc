/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    sat-fl-application.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-11-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-fl-application.h"
#include "ns3/uinteger.h"


namespace ns3
{

    TypeId SatelliteFLApplication::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteFLApplication")
            .SetParent<Application>()
            .SetGroupName("Application")
            .AddConstructor<SatelliteFLApplication>()
            .AddAttribute(
                "Port",
                "Set Application Upd Port",
                UintegerValue(2411),
                MakeUintegerAccessor(&SatelliteFLApplication::SetPort, &SatelliteFLApplication::GetPort),
                MakeUintegerChecker<uint16_t>()
            )
        ;

        return tid;
    }


    SatelliteFLApplication::SatelliteFLApplication()
    : m_init(false)
    {
    }


    SatelliteFLApplication::~SatelliteFLApplication()
    {
    }



    void SatelliteFLApplication::SetPort(const uint16_t port)
    {
        m_port = port;
    }


    uint16_t SatelliteFLApplication::GetPort() const
    {
        return m_port;
    }


    uint64_t SatelliteFLApplication::GetRxN() const
    {
        return m_srv->GetReceived();
    }


    uint32_t SatelliteFLApplication::GetLostRx() const
    {
        return m_srv->GetLost();
    }


    uint64_t SatelliteFLApplication::GetTxN() const
    {
        return m_clt->GetTotalTx();
    }


    void SatelliteFLApplication::Initialize()
    {
        m_clt = CreateObject<UdpClient>();
        m_srv = CreateObject<UdpServer>();

        m_init = true;
    }


    bool SatelliteFLApplication::IsInitialized() const
    {
        return m_init;
    }



}   /* namespace ns3 */