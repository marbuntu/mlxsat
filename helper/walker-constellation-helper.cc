/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */


#include <ns3/log.h>
#include <ns3/double.h>
#include <ns3/integer.h>

#include "walker-constellation-helper.h"

NS_LOG_COMPONENT_DEFINE("WalkerConstellationHelper");

namespace ns3 
{

NS_OBJECT_ENSURE_REGISTERED(WalkerConstellationHelper);

TypeId WalkerConstellationHelper::GetTypeId (void) 
{
    static TypeId tid = TypeId("ns3::WalkerConstellationHelper")
        .SetParent<Object>()
        .AddConstructor<WalkerConstellationHelper>()
        .AddAttribute("Inclination",
                      "Inclination of the orbital Plane",
                      DoubleValue(66.6),
                      MakeDoubleAccessor(&WalkerConstellationHelper::m_inclination),
                      MakeDoubleChecker<double>(0.0, 90.0))
        .AddAttribute("NumOfSats",
                      "Number of Satellites per Orbit",
                      IntegerValue(8),
                      MakeIntegerAccessor(&WalkerConstellationHelper::m_numSats),
                      MakeIntegerChecker<int>(1))
        .AddAttribute("NumOfOrbits",
                      "Number of orbital Planes",
                      IntegerValue(5),
                      MakeIntegerAccessor(&WalkerConstellationHelper::m_numPlanes),
                      MakeIntegerChecker<int>(1))
        .AddAttribute("Phasing",
                      "Walker Phasing Parameter",
                      DoubleValue(0.0),
                      MakeDoubleAccessor(&WalkerConstellationHelper::m_phasing),
                      MakeDoubleChecker<double>(0.0))
        .AddAttribute("RaanShift",
                      "Longitudal Rotation of the constellation",
                      DoubleValue(0.0),
                      MakeDoubleAccessor(&WalkerConstellationHelper::m_raanShift),
                      MakeDoubleChecker<double>(0.0, 360.0))
    ;
    return tid;
}

TypeId WalkerConstellationHelper::GetInstanceTypeId (void) const
{
    NS_LOG_FUNCTION(this);
    return GetTypeId();
}

WalkerConstellationHelper::WalkerConstellationHelper (void) 
{
    NS_LOG_FUNCTION(this);
}

void WalkerConstellationHelper::Initialize (void) 
{
    NS_LOG_FUNCTION(this);

    double raan = m_raanShift;
    double delta_raan = 360.0 / m_numPlanes;

    long N = 0;

    for (int i = 0; i < m_numPlanes; i++)
    {

        Ptr<WalkerOrbitHelper> orb = CreateObjectWithAttributes<WalkerOrbitHelper> (
            "Inclination", DoubleValue(m_inclination),
            "NumOfSats", IntegerValue(m_numSats),
            "AscendingNode", DoubleValue(raan)
        );

        orb->Initialize(N);
        m_orbits.push_back(orb);


        // Increment Satellite No 
        N += orb->getSatelliteCount();

        // Calc RAAN of next Orbit
        if ((raan += delta_raan) >= 360.0)
            raan -= 360.0;

    }

    m_totSats = N;

}

void WalkerConstellationHelper::LogInitialPositions (const std::string prefix, const std::string postfix)
{

    std::ostringstream oss;
    int n = 1;

    for (auto orb : m_orbits)
    {
        oss.str("");
        oss << prefix << "-orb" << n << postfix;

        orb->LogInitialPositions(oss.str());

        n++;
    }

}


long WalkerConstellationHelper::getSatelliteCount (void)
{
    return m_totSats;
}

};  // namsepace ns3
