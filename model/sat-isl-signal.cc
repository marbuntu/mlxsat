/**
 * @brief   Signal Class to hold Parameters for ISLs
 * 
 * @file    sat-isl-signal.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-signal.h"


namespace ns3
{


    TypeId SatelliteISLSignal::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLSignal")
            .SetParent<Object>()
        ;

        return tid;
    }


    TypeId SatelliteISLSignal::GetInstanceTypeId() const
    {
        return GetTypeId();
    }


    SatelliteISLSignal::SatelliteISLSignal(Ptr<Packet> pck)
    : AoA(0.0, 0.0)
    , AoD(0.0, 0.0)
    , m_pck(pck)
    {

    }


    SatelliteISLSignal::~SatelliteISLSignal()
    {
    }


    void SatelliteISLSignal::SetRateEstimate(DataRate rate)
    {
        m_estimate = rate;
    }


}   /* namespace ns3 */