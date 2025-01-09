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
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "math.h"
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
                , "Opening Angle of the Antenna Aparture from 0-180 Degree"
                , DoubleValue(180.0)
                , MakeDoubleAccessor(&SatelliteISLAntenna::SetOpeningAngle, &SatelliteISLAntenna::GetOpeningAngle)
                , MakeDoubleChecker<double>(0.0, 360.0)
            )
            .AddAttribute(
                "PointingErrorModel"
                , "Random Number Generator to use for Pointing-Loss Error"
                , StringValue("ns3::NormalRandomVariable")
                , MakePointerAccessor(&SatelliteISLAntenna::m_errmodel)
                , MakePointerChecker<RandomVariableStream>()
            )
            .AddAttribute(
                "PointingErrorScalingFactor"
                , "Factor to Scale generated random number for Pointing Error"
                , DoubleValue(1e-3)
                , MakeDoubleAccessor(&SatelliteISLAntenna::m_errfactor)
                , MakeDoubleChecker<double>()
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


    void SatelliteISLAntenna::SetOpeningAngle(double angle)
    {
        double half = WrapTo360(angle) * 0.5;
        m_openingAngle = DegreesToRadians(half);
    }


    double SatelliteISLAntenna::GetOpeningAngle() const
    {
        return RadiansToDegrees(m_openingAngle);
    }



    double SatelliteISLAntenna::GetGainDb(Angles angles)
    {
        //NS_LOG_UNCOND("Angle: " << angles.GetAzimuth());

        
        if (abs(angles.GetAzimuth()) > m_openingAngle)
        {
            return std::numeric_limits<double>::infinity() * -1;
        }

        double factor = 0.0;

        switch(m_pattern)
        {
            case RP_Cosine:
                factor = std::cos(abs(angles.GetAzimuth()));
                break;

            case RP_Bessel:
                factor = m_maxGainDbi;
                break;

            case RP_Constant:
                factor = m_maxGainDbi;
                break;
        }


        double gainDb = (10 * std::log10(factor)) + m_maxGainDbi;

        return gainDb;
    }


    double SatelliteISLAntenna::GetPointingErrorDb(const double gain_db) const
    {
        double gain = std::pow(10, gain_db / 10);
        double psi = DegreesToRadians(m_errmodel->GetValue() * m_errfactor);
        double err = exp( gain * std::pow(psi, 2));
        return 10 * std::log10(err);
    }


    Ptr<RandomVariableStream> SatelliteISLAntenna::GetPointingErrorModel()
    {
        return m_errmodel;
    }


}   /* namespace ns3 */