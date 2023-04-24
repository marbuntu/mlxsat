/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/**
 * @file    groundstation-helper.h
 * @author  Martin Anschuetz
 * @brief 
 * @version 0.1
 * @date    2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once

#include <ns3/object.h>
#include <ns3/mobility-model.h>

namespace ns3
{

class GroundstationHelper : public Object
{
public:

    static TypeId GetTypeId (void);


    TypeId GetInstanceTypeId (void) const;


    GroundstationHelper (void);

    virtual ~GroundstationHelper (void) 
    {
    }


    Vector GetCartesianCoordinates (void) const;


    Ptr<MobilityModel> GetMobilityModel (void) const;


private:

    double m_latitude;
    double m_longitude;
    double m_altitude;

};


}   // namespace ns3
