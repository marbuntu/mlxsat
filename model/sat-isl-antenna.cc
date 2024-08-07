/**
 * @brief   Satellite ISL Antenna Model
 * 
 * @file    sat-isl-antenna.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/double.h"

#include "sat-isl-antenna.h"


namespace ns3
{
    NS_OBJECT_ENSURE_REGISTERED(SatelliteISLAntenna);
    NS_LOG_COMPONENT_DEFINE("SatelliteISLAntenna");


    TypeId SatelliteISLAntenna::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLAntenna")
            .SetParent<AntennaModel>()
            .AddConstructor<SatelliteISLAntenna>()
            .AddAttribute(
                "RadiationPattern"
                , "Selects the Function to Model the of the Radiation Pattern."
                , EnumValue(RP_Cosine)
                , MakeEnumAccessor(&SatelliteISLAntenna::m_pattern)
                , MakeEnumChecker(
                    RP_Cosine, "Cosine",
                    RP_Bessel, "Bessel",
                    RP_Constant, "Constant"
                )
            )
            .AddAttribute(
                "MaxGainDbi"
                , "Set the max. Gain in dBi."
                , DoubleValue(0.0)
                , MakeDoubleAccessor(&SatelliteISLAntenna::m_maxGainDbi)
                , MakeDoubleChecker<double>(0.0)
            )
            .AddAttribute(
                "OpeningAngle"
                , "Opening Angle of the Antenna Aparture from 0-360 Degree"
                , DoubleValue(180.0)
                , MakeDoubleAccessor(&SatelliteISLAntenna::m_openingAngle)
                , MakeDoubleChecker<double>(0.0, 360.0)
            )
        ;

        return tid;
    }


    SatelliteISLAntenna::SatelliteISLAntenna()
    {
    }

    SatelliteISLAntenna::~SatelliteISLAntenna()
    {
    }


    double SatelliteISLAntenna::GetGainDb(Angles angles)
    {

        switch(m_pattern)
        {
            case RP_Cosine:
                return m_maxGainDbi * cos(angles.GetInclination());


            case RP_Bessel:
                return m_maxGainDbi;


            case RP_Constant:
                return m_maxGainDbi;

        }


        return 0;
    }


}   /* namespace ns3 */