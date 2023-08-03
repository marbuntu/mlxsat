/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */


#include <ns3/log.h>
#include <ns3/names.h>
#include <ns3/double.h>
#include <ns3/integer.h>
#include <ns3/type-id.h>

#include "walker-orbit-helper.h"
#include "ns3/pseudo-tle.h"


NS_LOG_COMPONENT_DEFINE("WalkerOrbitHelper");

namespace ns3 
{

NS_OBJECT_ENSURE_REGISTERED(WalkerOrbitHelper);


TypeId WalkerOrbitHelper::GetTypeId (void) 
{
    static TypeId tid = TypeId("ns3::WalkerOrbitHelper")
        .SetParent<Object>()
        .AddConstructor<WalkerOrbitHelper>()
        .AddAttribute("Inclination",
                      "Inclination of the Orbital Plane",
                      DoubleValue(30.0),
                      MakeDoubleAccessor(&WalkerOrbitHelper::m_inclination),
                      MakeDoubleChecker<double>(0.0, 90.0))
        .AddAttribute("NumOfSats",
                      "Number of Satellites per Orbit",
                      IntegerValue(1),
                      MakeIntegerAccessor(&WalkerOrbitHelper::m_numSats),
                      MakeIntegerChecker<int>())
        .AddAttribute("Phase",
                      "Phase Offset",
                      DoubleValue(0.0),
                      MakeDoubleAccessor(&WalkerOrbitHelper::m_phase),
                      MakeDoubleChecker<double>(0.0, 360.0))
        .AddAttribute("AscendingNode",
                      "Right Ascension of Ascending Node",
                      DoubleValue(0.0),
                      MakeDoubleAccessor(&WalkerOrbitHelper::m_raan),
                      MakeDoubleChecker<double>(0.0, 360.0))
        .AddAttribute("MeanMotion",
                      "Satellite Mean Motion",
                      DoubleValue(13.0),
                      MakeDoubleAccessor(&WalkerOrbitHelper::m_meanMotion),
                      MakeDoubleChecker<double>(0.0, 17.0))
    ;

    return tid;
}

TypeId WalkerOrbitHelper::GetInstanceTypeId (void) const
{
    NS_LOG_FUNCTION(this);
    return GetTypeId();
}

WalkerOrbitHelper::WalkerOrbitHelper (void) 
{
    NS_LOG_FUNCTION (this);

    // Is this neccessary for Abstraction of Object-Class ?
    ObjectBase::ConstructSelf (AttributeConstructionList ());
    
    
}

void WalkerOrbitHelper::Initialize (unsigned long satNo_seed)
{
    NS_LOG_FUNCTION (this);

    // Phase delta between satellites
    double phase_delta = 360.0 / m_numSats;

    // Phase shift or the orbit: (rotated)
    double phase = m_phase;

    for (unsigned long n = satNo_seed; n < satNo_seed + m_numSats; n++)
    {
        Ptr<PseudoSatTLE> sat = CreateObjectWithAttributes<PseudoSatTLE>(
            "Inclination", DoubleValue(m_inclination),
            "SatNo", IntegerValue(n),
            "MeanAnomaly", DoubleValue(phase),
            "AscendingNode", DoubleValue(m_raan),
            "MeanMotion", DoubleValue(m_meanMotion)
        );

        m_satFactory.SetTypeId("ns3::SatSGP4MobilityModel");
        Ptr<SatSGP4MobilityModel> sat_mob = m_satFactory.Create<SatSGP4MobilityModel>();
        sat_mob->SetTleInfo(sat->TleFull());

        m_sats.push_back(sat_mob);

        
        Ptr<Node> node = CreateObject<Node>();
        node->AggregateObject(sat_mob);

        m_nodes.Add(node);

        // Calculate next Phase
        ;
        if ((phase += phase_delta) > 360.0) 
            phase -= 360.0;

    }
    
}


void WalkerOrbitHelper::LogInitialPositions(const std::string filename) 
{

    std::ofstream outp(filename);

    for (auto sat : m_sats)
    {
        GeoCoordinate geo = GeoCoordinate(sat->GetPosition());
        outp << geo.GetLatitude()
            << "    " << geo.GetLongitude()
            << "    " << geo.GetAltitude()
            << std::endl;
    } 

    outp.close();
}

int WalkerOrbitHelper::getSatelliteCount (void) const
{
    return m_numSats;
}


Ptr<MobilityModel> WalkerOrbitHelper::getSatellite (unsigned int satIndex ) const 
{
    if (satIndex > m_sats.size())
        return nullptr;
        
    return m_sats.at(satIndex);
}

}   // namespace ns3
