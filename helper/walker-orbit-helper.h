/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/**
 * @file    walker-orbit-helper.h
 * @author  Martin Anschuetz
 * @brief 
 * @version 0.1
 * @date    2023-04-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <ns3/node-container.h>
#include <ns3/object-factory.h>
#include <ns3/object.h>
#include <ns3/satellite-sgp4-mobility-model.h>


namespace ns3 
{


class WalkerOrbitHelper : public Object
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
    TypeId GetInstanceTypeId(void) const;

    /**
     * @brief Create Base SatOrbitHelper for creating customized Sat orbits
     */
    WalkerOrbitHelper();

    /**
     * @brief Create Base SatOrbitHelper using Set of TLE Data
     * 
     * @param tle_set 
     */
    //WalkerOrbitHelper(std::vector<std::string> &tle_set);

    /**
     * @brief Destructor for SatOrbitHelper 
     * 
     */
    virtual ~WalkerOrbitHelper()
    {
    }

    void Initialize (unsigned long satNo_seed = 0);

    void LogInitialPositions (const std::string filename);

    int getSatelliteCount (void) const;


    /**
     * @brief Get the Satellite Object
     * 
     * @param satIndex  Satellite Index (starting at 0)
     * @return Ptr<MobilityModel> 
     */
    Ptr<MobilityModel> getSatellite(unsigned int satIndex) const;

private:

    double m_inclination;
    int m_numSats;
    double m_phase;
    double m_raan;
    double m_meanMotion;

    std::vector<Ptr<SatSGP4MobilityModel>> m_sats;
    NodeContainer m_nodes;
    ObjectFactory m_satFactory;

};


}   // namespace ns3
