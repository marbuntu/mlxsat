/**
 * @brief   HEADER TEMPLATE
 * 
 * @file    sat-fl-application.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-11-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef HEADER_TEMPLATE_H
#define HEADER_TEMPLATE_H


#include "ns3/application.h"
#include "ns3/udp-client.h"
#include "ns3/udp-server.h"



namespace ns3
{

    class SatelliteFLApplication : public Application
    {
    public:

        static TypeId GetTypeId();


        SatelliteFLApplication();
        ~SatelliteFLApplication();


        void CancelEvents();


        void SetPort(const uint16_t port);
        uint16_t GetPort() const;

        
        uint64_t GetRxN() const;
        uint32_t GetLostRx() const;

        uint64_t GetTxN() const;


        /**
         * @brief Initialize FL Application
         * 
         */
        void Initialize();

        /**
         * @brief Check if FL Application is Initialized
         * 
         * @return true 
         * @return false 
         */
        bool IsInitialized() const;


    protected:
    private:

        bool m_init;

        uint16_t m_port;

        Ptr<UdpServer> m_srv;
        Ptr<UdpClient> m_clt;        


    };  /* SatelliteFLApplication */


};  /* namespace ns3 */


#endif /* HEADER_TEMPLATE_H */