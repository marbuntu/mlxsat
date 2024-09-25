/**
 * @brief   Helper to setup Satellite ISL Interfaces
 * 
 * @file    sat-isl-interface-helper.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-09-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "ns3/double.h"
#include "ns3/enum.h"
#include "ns3/sat-isl-interface-helper.h"
#include "ns3/sat-isl-antenna.h"
#include "ns3/sat-isl-terminal.h"
#include "ns3/sat-isl-net-device.h"


namespace ns3
{

    TypeId SatelliteISLInterfaceHelper::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLInterfaceHelper")
            .SetParent<Object>()
            .AddConstructor<SatelliteISLInterfaceHelper>()
        ;

        return tid;
    }


    SatelliteISLInterfaceHelper::SatelliteISLInterfaceHelper()
    {
    }


    SatelliteISLInterfaceHelper::~SatelliteISLInterfaceHelper()
    {
        m_terminalFactories.clear();
    }


    Ptr<SatelliteISLNetDevice> SatelliteISLInterfaceHelper::Create() const
    {
        Ptr<SatelliteISLNetDevice> itf = CreateObject<SatelliteISLNetDevice>();
        itf->SetAddress(Mac48Address::Allocate());

        for (auto &tfac : m_terminalFactories)
        {
            // Create a new Terminal from Config (incl. Antenna Model)
            itf->RegisterISLTerminal(
                tfac->Create()
            );
        }

        return itf;
    }


    Ptr<SatelliteISLNetDevice> SatelliteISLInterfaceHelper::CreateAndAggregate(Ptr<Node> node, const Ptr<SatelliteISLChannel> channel) const
    {
        Ptr<SatelliteISLNetDevice> itf = Create();
        itf->SetChannel(channel);
        itf->SetNode(node);
        node->AddDevice(itf);

        return itf;
    }


    SatelliteISLInterfaceHelper DefaultISLInterfaceSetup::GetDefaultFactory(setup_t setup)
    {
        switch (setup)
        {
        case ASYMMETRIC_2x2:
            return _Asymmetric2x2();
        
        case SYMMETRIC_4x:
        default:
            return _Symmetric4x();
        }
        
    }


    SatelliteISLInterfaceHelper DefaultISLInterfaceSetup::_Symmetric4x() 
    {

        SatelliteISLInterfaceHelper itf_helper;

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(0.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(50.0),
                "OpeningAngle", DoubleValue(160.0)
            );

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(90.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(50.0),
                "OpeningAngle", DoubleValue(160.0)
            );

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(180.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(50.0),
                "OpeningAngle", DoubleValue(160.0)
            );

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(-90.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(50.0),
                "OpeningAngle", DoubleValue(160.0)
            );

        return itf_helper;
    }


    SatelliteISLInterfaceHelper DefaultISLInterfaceSetup::_Asymmetric2x2() 
    {

        SatelliteISLInterfaceHelper itf_helper;

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(0.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(50.0),
                "OpeningAngle", DoubleValue(120.0)
            );

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(90.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(30.0),
                "OpeningAngle", DoubleValue(160.0)
            );

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(180.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(50.0),
                "OpeningAngle", DoubleValue(120.0)
            );

        itf_helper.AddTerminal(
            "Phi", DoubleValue(0.0),
            "Theta", DoubleValue(0.0),
            "Psi", DoubleValue(-90.0)
            )->SetAntennaModel<SatelliteISLAntenna>(
                "RadiationPattern", EnumValue(SatelliteISLAntenna::RP_Cosine),
                "MaxGainDbi", DoubleValue(30.0),
                "OpeningAngle", DoubleValue(160.0)
            );

        return itf_helper;
    }


}   /* namespace ns3 */