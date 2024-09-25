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
            .AddConstructor<SatelliteISLSignal>()
        ;

        return tid;
    }


    TypeId SatelliteISLSignal::GetInstanceTypeId() const
    {
        return GetTypeId();
    }


    SatelliteISLSignal::SatelliteISLSignal()
    {
    }


    SatelliteISLSignal::~SatelliteISLSignal()
    {
    }


}   /* namespace ns3 */