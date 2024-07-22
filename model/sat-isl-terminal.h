/**
 * @brief   Inter-Satellite-Link Terminal
 * 
 * @file    sat-isl-terminal.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_ISL_TERMINAL_H
#define SATELLITE_ISL_TERMINAL_H


#include "ns3/object.h"


namespace ns3
{


    typedef enum {
        RxOnly,
        TxOnly,
        RxTx
    } SatISLTerminalMode_t;


class SatelliteISLTerminal : public Object
{
public:

    static TypeId GetTypeId();
    TypeId GetInstanceTypeId() const;

    SatelliteISLTerminal();
    ~SatelliteISLTerminal();


private:

    SatISLTerminalMode_t m_phyMode;
    bool m_sharedNetDevice;


}; /* SatelliteISLTerminal */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_TERMINAL_H */