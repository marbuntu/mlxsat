/**
 * @brief   Satellite Node Tag for additional Information
 * 
 * @file    sat-node-tag.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-26-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_NODE_TAG_H
#define SATELLITE_NODE_TAG_H

#include "ns3/object.h"
#include "ns3/node.h"
#include <unordered_map>


namespace ns3
{

    class SatelliteNodeTag : public Object
    {
    public:

        typedef std::unordered_map<uint8_t, std::vector<uint16_t>>              ConstOrbitMap;
        typedef std::unordered_map<uint8_t, std::vector<uint16_t>>::iterator    ConstOrbitMapIter;

        typedef std::unordered_map<uint16_t, std::vector<uint16_t>>             OrbitSatMap;
        typedef std::unordered_map<uint16_t, std::vector<uint16_t>>::iterator   OrbitSatMapIter;

        typedef std::unordered_map<uint16_t, Ptr<SatelliteNodeTag>>             SatNodeMap;
        typedef std::unordered_map<uint16_t, Ptr<SatelliteNodeTag>>::iterator   SatNodeMapIter;


        static TypeId GetTypeId();

        /**
         * @brief Get Number of Registered Constellations
         * 
         * @return uint16_t 
         */
        static uint16_t GetConstellationsN();

        /**
         * @brief Get Number of Registered Orbits
         * 
         * @return uint16_t 
         */
        static uint16_t GetOrbitsN();

        /**
         * @brief Get Number of Registered Orbits by Constellation
         * 
         * @param constellationID 
         * @return uint16_t 
         */
        static uint16_t GetOrbitsN(const uint8_t constellationID);

        /**
         * @brief Get Number of Registered Satellites
         * 
         * @return uint16_t 
         */
        static uint16_t GetSatsN();

        /**
         * @brief Get Number of Registered Satellites by Orbit
         * 
         * @param orbitID 
         * @return uint16_t 
         */
        static uint16_t GetSatsN(const uint16_t orbitID);


        static std::vector<uint16_t> SatsByOrbit(const uint16_t orbitID);
        static std::vector<uint16_t> OrbitsByConstellation(const uint8_t constellationID);
        
        static Ptr<Node> GetSatellite(const uint16_t id);



        SatelliteNodeTag();
//        SatelliteNodeTag(size_t ConstellationID, size_t Orbit, size_t SatNo);
        ~SatelliteNodeTag();

        void Register();
        void Register(const uint8_t constellationID, const uint16_t orbitID);

        uint16_t GetId() const;

        uint16_t GetOID() const;

        uint8_t GetCID() const;


    protected:

        //typedef std::unordered_map<uint16_t, Ptr<SatelliteNodeTag>> 

        uint8_t m_CID;      //! Constellation ID
        uint16_t m_OID;      //! Orbit ID

        uint16_t m_id;


        static ConstOrbitMap m_CmapO;
        static OrbitSatMap m_OmapS;
        static SatNodeMap m_globalNodes;

    };  /* SatelliteNodeTag */


};  /* namespace ns3 */


#endif /* SATELLITE_NODE_TAG_H */