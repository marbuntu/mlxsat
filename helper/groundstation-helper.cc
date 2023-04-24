/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/**
 * @file    groundstation-helper.cc
 * @author  Martin Anschuetz
 * @brief 
 * @version 0.1
 * @date    2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <ns3/log.h>
#include <ns3/type-id.h>
#include <ns3/double.h>
#include <ns3/vector.h>
#include <ns3/math.h>
#include <ns3/constant-position-mobility-model.h>

#include "groundstation-helper.h"

NS_LOG_COMPONENT_DEFINE("GroundStationHelper");

namespace ns3 
{

NS_OBJECT_ENSURE_REGISTERED(GroundstationHelper);

TypeId GroundstationHelper::GetTypeId (void) 
{
    static TypeId tid = TypeId("ns3::GroundStationHelper")
        .SetParent<Object>()
        .AddConstructor<GroundstationHelper>()
        .AddAttribute("Latitude",
                      "Position Coordinate Latitude in degree",
                      DoubleValue(0.0),
                      MakeDoubleAccessor(&GroundstationHelper::m_latitude),
                      MakeDoubleChecker<double>(-90.0, 90.0))
        .AddAttribute("Longitude",
                      "Position Coordinate Longitude in degree",
                      DoubleValue(0.0),
                      MakeDoubleAccessor(&GroundstationHelper::m_longitude),
                      MakeDoubleChecker<double>(-180.0, 180.0))
        .AddAttribute("Altitude",
                      "Position Altitude in meters",
                      DoubleValue(0.0),
                      MakeDoubleAccessor(&GroundstationHelper::m_altitude),
                      MakeDoubleChecker<double>(0.0))
    ;
    return tid;
}

TypeId GroundstationHelper::GetInstanceTypeId (void) const
{
    NS_LOG_FUNCTION(this);
    return GetTypeId();
}

GroundstationHelper::GroundstationHelper (void)
{
    NS_LOG_FUNCTION(this);
}


Vector GroundstationHelper::GetCartesianCoordinates (void) const
{
    double rad = m_altitude +  6.371e6;
    double lat = m_latitude * (M_PI / 180);
    double lon =  m_longitude * (M_PI / 180);
    Vector pos = Vector3D (rad * cos (lat) * cos (lon),
  			   rad * cos (lat) * sin (lon),
  			   rad * sin (lat));

    return pos;
}

Ptr<MobilityModel> GroundstationHelper::GetMobilityModel (void) const
{
    Ptr<ConstantPositionMobilityModel> mob = CreateObject<ConstantPositionMobilityModel> ();
    mob->SetPosition(GetCartesianCoordinates());

    return mob;
}

}   // namespace ns3
