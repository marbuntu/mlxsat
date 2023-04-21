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

    void Install (WalkerOrbitHelper &orbit);

    void Initialize (void);

    void LogInitialPositions (const std::string prefix, const std::string postfix);

    long getSatelliteCount (void);

private:

    double m_inclination;
    int m_numPlanes;
    int m_numSats;
    double m_phasing;
    double m_raanShift;

    long m_totSats;

    std::vector<Ptr<WalkerOrbitHelper>> m_orbits;

};


};   // namespace ns3
