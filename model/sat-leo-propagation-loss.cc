/**
 * @brief   Propagation Loss Model for Inter-Satellite-Links in LEO Constellations
 * 
 * @file    sat-leo-propagation-loss.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-leo-propagation-loss.h"


#include "ns3/double.h"

namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("SatellitePropagationLossLEO");
    NS_OBJECT_ENSURE_REGISTERED(SatellitePropagationLossLEO);


    TypeId SatellitePropagationLossLEO::GetTypeId() 
    {
        static TypeId tid = TypeId("ns3::SatellitePropagationLossLEO")
            .SetParent<PropagationLossModel>()
            .AddConstructor<SatellitePropagationLossLEO>()
            .AddAttribute(
                "DefaultFc"
                , "Default Communication Center Frequency in Hz"
                , DoubleValue(1e9)
                , MakeDoubleAccessor(&SatellitePropagationLossLEO::m_fc)
                , MakeDoubleChecker<double>(1.0e6)
            )
        ;

        return tid;
    }


    SatellitePropagationLossLEO::SatellitePropagationLossLEO()
    : m_fc(0.0)
    {
    }


    SatellitePropagationLossLEO::~SatellitePropagationLossLEO()
    {
    }


    double SatellitePropagationLossLEO::DoCalcRxPower(double txPowerDbm, Ptr<MobilityModel>, Ptr<MobilityModel>) const
    {
        return 0.0;
    }


    int64_t SatellitePropagationLossLEO::DoAssignStreams(int64_t stream)
    {
        return 1;
    }


    void SatellitePropagationLossLEO::SetCenterFrequency(double fc)
    {
        NS_LOG_FUNCTION(this << fc);
        m_fc = fc;
    }


    double SatellitePropagationLossLEO::GetCenterFrequency() const
    {
        NS_LOG_FUNCTION(this);
        return m_fc;
    }


}   /* namespace ns3 */