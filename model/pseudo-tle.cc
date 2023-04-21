/**
 * @file pseudo-tle.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-11
 * 
 * @copyright Copyright (c) 2023
 * 
 * Build pseudo TLE description for a given trajectory
 * Serialize to String to pass to other classes
 * 
 */

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>

#include <ns3/log.h>
#include <ns3/names.h>
#include <ns3/integer.h>
#include <ns3/double.h>
#include <ns3/simulator.h>

#include "pseudo-tle.h"


NS_LOG_COMPONENT_DEFINE ("pseudo-sat-tle");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(PseudoSatTLE);

std::string make_scientific_tle(double value) 
{

    int N = (int) ceil(log10(value));

    std::cout << N << std::endl;

    return "";
}

TypeId PseudoSatTLE::GetTypeId (void) 
{
    static TypeId tid = TypeId("ns3::PseudoSatTLE")
        .SetParent<Object>()
        .AddConstructor<PseudoSatTLE> ()
        .AddAttribute ("SatNo",
                        "Satellite Number",
                        IntegerValue(0),
                        MakeIntegerAccessor(&PseudoSatTLE::m_satNo),
                        MakeIntegerChecker<int>())
        .AddAttribute("Inclination",
                        "Inclination of the Orbital Plane",
                        DoubleValue(30.0),
                        MakeDoubleAccessor(&PseudoSatTLE::m_inclination),
                        MakeDoubleChecker<double>(0.0, 90.0))
        .AddAttribute("AscendingNode",
                        "Right Ascension of the Ascending Node",
                        DoubleValue(0.0),
                        MakeDoubleAccessor(&PseudoSatTLE::m_raan),
                        MakeDoubleChecker<double>(0.0, 360.0))
        .AddAttribute("Perigee",
                        "Argument of the Perigee",
                        DoubleValue(-90.0),
                        MakeDoubleAccessor(&PseudoSatTLE::m_perigee),
                        MakeDoubleChecker<double>(0.0, 360.0))
        .AddAttribute("MeanAnomaly",
                        "Mean Anomaly",
                        DoubleValue(0.0),
                        MakeDoubleAccessor(&PseudoSatTLE::m_meanAnomaly),
                        MakeDoubleChecker<double>(0.0, 360.0))
        .AddAttribute("MeanMotion",
                        "Mean Motion (theoretically 0-17)",
                        DoubleValue(15.0),
                        MakeDoubleAccessor(&PseudoSatTLE::m_meanMotion),
                        MakeDoubleChecker<double>(0.0, 17.0))
        .AddAttribute("Eccentricity",
                        "Eccentricity",
                        DoubleValue(0.0),
                        MakeDoubleAccessor(&PseudoSatTLE::m_eccentricty),
                        MakeDoubleChecker<double>())
    ;
    return tid;
}

TypeId PseudoSatTLE::GetInstanceTypeId (void) const
{
    return GetTypeId();
}


PseudoSatTLE::PseudoSatTLE (void)
    : m_satNo(0),
    m_launchYear(23),
    m_launchNo(1),
    m_epochYear(23),
    m_epochDay(79.89166667),
    m_inclination(30.0),
    m_raan(7.4581),
    m_eccentricty(0.0),
    m_meanAnomaly(0.0),
    m_meanMotion(15.0),
    m_dtmm(0.0),
    m_dt2mm(0.0),
    m_perigee(-90.0),
    m_revs(0)
{
    NS_LOG_FUNCTION (this);
    ObjectBase::ConstructSelf (AttributeConstructionList ());
}

std::string PseudoSatTLE::TleLine1(void)
{
    std::ostringstream oss;
    oss << "1 ";
    oss << std::setfill('0') << std::setw(5) << m_satNo << "U ";
    oss << std::setfill('0') << std::setw(2) << m_launchYear;
    oss << std::setfill('0') << std::setw(3) << m_launchNo << "A  ";
    oss << std::setfill('0') << std::setw(2) << m_epochYear;
    oss << std::setfill('0') << std::setw(12) << std::fixed << std::setprecision(8) << m_epochDay << " ";
    // Add MM Derivatives and Drag Term
    oss << " -.00000000"  << "  00000+0 " << " 00000+0 0";
    oss << "  9990";
    
    return oss.str();
}

std::string PseudoSatTLE::TleLine2(void)
{
    std::ostringstream oss;
    oss << "2 ";
    oss << std::setfill('0') << std::setw(5) << m_satNo << " ";
    oss << std::setfill('0') << std::setw(8) << std::setprecision(4) << std::fixed << m_inclination << " ";
    oss << std::setfill('0') << std::setw(8) << std::setprecision(4) << std::fixed << m_raan << " ";
    // Add Eccentricity
    oss << "0000000 ";
    oss << std::setfill('0') << std::setw(8) << std::setprecision(4) << std::fixed << m_perigee << " ";
    oss << std::setfill('0') << std::setw(8) << std::setprecision(4) << std::fixed << m_meanAnomaly << " ";
    oss << std::setfill('0') << std::setw(11) << std::setprecision(8) << std::fixed << m_meanMotion;
    oss << std::setfill('0') << std::setw(5) << std::fixed << m_revs << "0";
    return oss.str();
}

std::string PseudoSatTLE::TleFull(void)
{
    return this->TleLine1() + "\n" + this->TleLine2();;
}


}   // namespace ns3