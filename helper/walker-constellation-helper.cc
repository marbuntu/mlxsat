/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */


#include <ns3/log.h>
#include <ns3/double.h>
#include <ns3/integer.h>

#include <math.h>

#include "walker-constellation-helper.h"

NS_LOG_COMPONENT_DEFINE("WalkerConstellationHelper");

namespace ns3 
{

NS_OBJECT_ENSURE_REGISTERED(WalkerConstellationHelper);


const double WalkerConstellationHelper::productGM = 3.9860e14;
const double WalkerConstellationHelper::secsPerDay =86400.0;
const double WalkerConstellationHelper::twoPi = 6.2831853072;
const double WalkerConstellationHelper::meanEarthRadius = 6371.0;


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
        .AddAttribute("Altitude",
                      "Orbit Altitude in km",
                      DoubleValue(860.0),
                      MakeDoubleAccessor(&WalkerConstellationHelper::m_altitude),
                      MakeDoubleChecker<double>(100.0))
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
            "AscendingNode", DoubleValue(raan),
            "MeanMotion", DoubleValue(getMeanMotion())
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

double WalkerConstellationHelper::getGeocentricRadius (void) const
{
    return (meanEarthRadius + m_altitude) * 1000.0;
}

double WalkerConstellationHelper::getOrbitalPeriod (void) const
{
    // T = 2 π √ ( r^3 / (G M ) )
    return twoPi * sqrt( pow(getGeocentricRadius(), 3) / productGM );
}

double WalkerConstellationHelper::getMeanMotion (void) const
{
    // n = 86400 / T
    return secsPerDay / getOrbitalPeriod();
}

Ptr<MobilityModel> WalkerConstellationHelper::getSatellite (unsigned long satIndex) const
{
    if (satIndex > m_totSats)
        return nullptr;

    int orb =(int) (satIndex / m_numSats);
    unsigned int sat =(unsigned int) (satIndex - (m_numSats * orb));

    //std::cout << "Orb: " << orb << " Sat: " << sat << std::endl;

    return m_orbits.at(orb)->getSatellite(sat);
}


};  // namsepace ns3
