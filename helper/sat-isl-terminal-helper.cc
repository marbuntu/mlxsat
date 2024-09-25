/**
 * @brief   Satellite ISL Terminal Helper
 * 
 * @file    sat-isl-terminal-helper.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-09-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "ns3/sat-isl-terminal-helper.h"
#include "ns3/sat-isl-antenna.h"
#include "ns3/double.h"
#include "ns3/enum.h"


namespace ns3
{

    TypeId SatelliteISLTerminalHelper::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLTerminalHelper")
            .SetParent<Object>()
            .AddConstructor<SatelliteISLTerminalHelper>()
            .AddAttribute(
                "Phi"
                , "Roll Angle (around x) within [-180, 180]"
                , DoubleValue(0.0)
                , MakeDoubleAccessor(&SatelliteISLTerminalHelper::m_phi)
                , MakeDoubleChecker<double>(-180.0, 180.0)
            )
            .AddAttribute(
                "Theta"
                , "Pitch Angle (around y) within [-90, 90]"
                , DoubleValue(0.0)
                , MakeDoubleAccessor(&SatelliteISLTerminalHelper::m_theta)
                , MakeDoubleChecker<double>(-90.0, 90.0)
            )
            .AddAttribute(
                "Psi"
                , "Yaw Angle (around z) within [-180, 180]"
                , DoubleValue(0.0)
                , MakeDoubleAccessor(&SatelliteISLTerminalHelper::m_psi)
                , MakeDoubleChecker<double>(-180.0, 180.0)
            )
        ;

        return tid;
    }


    SatelliteISLTerminalHelper::SatelliteISLTerminalHelper()
    {
        m_terminalFactory.SetTypeId(SatelliteISLTerminal::GetTypeId());
        
        SetAntennaModel<SatelliteISLAntenna>(
            "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
            "MaxGainDbi", DoubleValue(30.0),
            "OpeningAngle", DoubleValue(170.0)
        );
    }


    SatelliteISLTerminalHelper::~SatelliteISLTerminalHelper()
    {
    }


    Ptr<SatelliteISLTerminal> SatelliteISLTerminalHelper::Create() const
    {
        Ptr<SatelliteISLTerminal> ter = StaticCast<SatelliteISLTerminal>(m_terminalFactory.Create());
        ter->SetAntennaModel(StaticCast<AntennaModel>(m_antennaFactory.Create()));
        ter->SetRelativeOrientation(m_phi, m_theta, m_psi);

        return ter;
    }


    // void SatelliteISLTerminalHelper::SetRelativeOrientation(const double degree_phi, const double degree_theta, const double degree_psi)
    // {
    //     m_phi = WrapTo180(degree_phi);
    //     m_theta = WrapTo
    // }



}   /* namespace ns3 */