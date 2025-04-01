/**
 * @brief   Signal Class to hold Parameters for ISLs
 * 
 * @file    sat-isl-signal.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_ISL_SIGNAL_H
#define SATELLITE_ISL_SIGNAL_H


#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/data-rate.h"
#include "ns3/angles.h"

namespace ns3
{


class SatelliteISLSignal : public Object
{
public:

    /**
     * @brief   Get the Class TypeId
     * 
     * @return  TypeId 
     */
    static TypeId GetTypeId();

    /**
     * @brief   Get the Class TypeId of an Instance
     * 
     * @return TypeId 
     */
    TypeId GetInstanceTypeId() const;

    /**
     * @brief Constructor of SatelliteISLSignal
     * 
     */
    SatelliteISLSignal(Ptr<Packet> pck);

    /**
     * @brief Copy Constructor of SatelliteISLSignal
     * 
     * @param other     Instance to Copy from
     */
    SatelliteISLSignal(const SatelliteISLSignal& other);

    /**
     * @brief Default Destructor
     * 
     */
    ~SatelliteISLSignal();


    // void SetPacket(const Ptr<Packet> pck);

    // void SetSrcNode(const Ptr<Node> scr);

    // void SetDstNode(const Ptr<Node> dst);

    void SetRateEstimate(DataRate rate);
    DataRate GetRateEstimate() const;

    double fc;            // Center Frequency
    double relVelocity;   // Relative Speed between nodes
    double txPower;       // Transmit Power in dBW

    double NoisePwrDb;
    double PointingError;

    double GetPacket();


    Angles AoA;
    Angles AoD;


private:
    Ptr<Packet> m_pck;

    DataRate m_estimate;

    // Ptr<Node> m_dst;
    // Ptr<Node> m_src;


};  /* SatelliteISLSignal */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_SIGNAL_H */