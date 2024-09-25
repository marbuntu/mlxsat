/**
 * @brief   Propagation Loss Model for Inter-Satellite-Links in LEO Constellations
 * 
 * @file    sat-leo-propagation-loss.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.de)
 * @version 1.0
 * @date    2024-07-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_LEO_PROPAGATION_LOSS_H
#define SATELLITE_LEO_PROPAGATION_LOSS_H

#include "ns3/mobility-model.h"
#include "ns3/propagation-loss-model.h"


namespace ns3
{


/**
 * @ingroup satellite
 * 
 * 
 * @brief Propagation Loss Model for ISLs in LEO Constellations
 * 
 */
class SatellitePropagationLossLEO : public PropagationLossModel
{
public:

    static TypeId GetTypeId();

    SatellitePropagationLossLEO();
    ~SatellitePropagationLossLEO();

    /**
     * @brief 
     * 
     * @param txPowerDbm 
     * @return double 
     */
    double DoCalcRxPower(double txPowerDbm, Ptr<MobilityModel>, Ptr<MobilityModel>) const;

    double DoCalcRxPower(double txPowerDbm, double fc, Ptr<MobilityModel> tx_mob, Ptr<MobilityModel> rx_mob);

    /**
     * @brief 
     * 
     * @param stream 
     * @return int64_t 
     */
    int64_t DoAssignStreams(int64_t stream);


    double DoCalcFSPL(Ptr<MobilityModel> tx_mob, Ptr<MobilityModel> rx_mob, double fc) const;


    /**
     * @brief Loss Factor due to Free Space Path Loss
     * 
     * @param tx_mod    Transmitter Mobility Model
     * @param rx_mob    Receiver Mobility Model
     * @param fc        Center Frequency
     * @return double   FSPL Loss Factor
     */
    double DoCalcFSPLFactor(Ptr<MobilityModel> tx_mod, Ptr<MobilityModel> rx_mob, double fc) const;


    /**
     * @brief Set the Communication Center Frequency in Hz
     * 
     * @param fc    fc in Hz
     */
    void SetCenterFrequency(double fc);


    /**
     * @brief Get the Communication Center Frequency in Hz
     * 
     * @return double   fc in Hz
     */
    double GetCenterFrequency() const;


private:

    /**
     * @brief Center Frequency
     * 
     */
    double m_fc;


};  /* SatelliteLEOPropagationLoss*/


};  /* namespace ns3 */


#endif /* SATELLITE_LEO_PROPAGATION_LOSS_H */