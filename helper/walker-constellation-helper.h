/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/**
 * @file    walker-constellation-helper.h
 * @author  Martin Anschütz
 * @brief   
 * @version 0.1
 * @date    2023-04-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once


#include <ns3/object.h>
#include <ns3/node-container.h>

#include "walker-orbit-helper.h"


namespace ns3 
{

class WalkerConstellationHelper : public Object
{
public:

    /**
     * @brief Constant: Seconds Per Day
     * 
     */
    static const double secsPerDay;

    /**
     * @brief Constant: Product of Gravitational Constant times Earth Mass (G*M)
     * 
     */
    static const double productGM;

    /**
     * @brief Constant: 2*Pi
     * 
     */
    static const double twoPi;

    /**
     * @brief Constant: Mean Earth Radius
     * 
     */
    static const double meanEarthRadius;


    /**
     * @brief Get the Type Id object
     * 
     * @return Object TypeId 
     */
    static TypeId GetTypeId (void);

    /**
     * @brief Get Type ID of object instance
     * @return Type ID of object instance
     */
    TypeId GetInstanceTypeId (void) const;

    /**
     * @brief Construct Base Walker Constellation Helper
     */
    WalkerConstellationHelper (void);

    virtual ~WalkerConstellationHelper()
    {
    }

    /**
     * @brief Manually add Orbit to constellation
     * 
     * @param orbit 
     */
    void Install (WalkerOrbitHelper &orbit);

    /**
     * @brief Run Constellation Setup
     * 
     */
    void Initialize (void);

    /**
     * @brief Log initial Satellite Positions to File
     * 
     * @param prefix 
     * @param postfix 
     */
    void LogInitialPositions (const std::string prefix, const std::string postfix);

    /**
     * @brief Get total Number of Satellites
     * 
     * @return long 
     */
    long getSatelliteCount (void);

    /**
     * @brief Get the Geocentric Radius (Altitude) of the Constellation
     * 
     * @return double 
     */
    double getGeocentricRadius (void) const;

    /**
     * @brief Get the Orbital Period
     * 
     * @return double 
     */
    double getOrbitalPeriod (void) const;

    /**
     * @brief Get the Mean Motion
     * 
     * @return double 
     */
    double getMeanMotion (void) const;

    /**
     * @brief Get the Satellite object
     * 
     * @param satIndex  Satellite Index (starting at 0)
     * @return Ptr<MobilityModel> 
     */
    Ptr<MobilityModel> getSatellite(unsigned long satIndex) const;

private:

    double m_inclination;
    double m_altitude;
    int m_numPlanes;
    int m_numSats;
    double m_phasing;
    double m_raanShift;

    unsigned long m_totSats;

    std::vector<Ptr<WalkerOrbitHelper>> m_orbits;

};


};   // namespace ns3
