/**
 * @brief   Inter-Satellite-Link Terminal
 * 
 * @file    sat-isl-terminal.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-terminal.h"

#include "ns3/enum.h"
#include "ns3/boolean.h"


namespace ns3
{




    TypeId SatelliteISLTerminal::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLTerminal")
            .SetParent<Object>()
            .AddConstructor<SatelliteISLTerminal>()
            .AddAttribute(
                "PhyMode",
                "Sets the mode of the Terminal to  RxOnly, TxOnly or RxTx",
                EnumValue(RxTx),
                MakeEnumAccessor(&SatelliteISLTerminal::m_phyMode),
                MakeEnumChecker(
                    RxOnly, "RxOnly",
                    TxOnly, "TxOnly",
                    RxTx, "RxTx"
                )
            )
            .AddAttribute(
                "SharedNetDevice",
                "Use one Shared Net Device (must be registered to instance),"
                "else the Terminal will create an internal NetDevice.",
                BooleanValue(false),
                MakeBooleanAccessor(&SatelliteISLTerminal::m_sharedNetDevice),
                MakeBooleanChecker()
            )
        ;

        return tid;
    }


    TypeId SatelliteISLTerminal::GetInstanceTypeId() const
    {
        return GetTypeId();
    }


    SatelliteISLTerminal::SatelliteISLTerminal()
    {
    }


    SatelliteISLTerminal::~SatelliteISLTerminal()
    {
    }



}   /* namespace ns3 */