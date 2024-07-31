/**
 * @brief   Inter-Satellite-Link Terminal
 * 
 * @file    sat-isl-terminal.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_ISL_TERMINAL_H
#define SATELLITE_ISL_TERMINAL_H


#include "ns3/object.h"

#include "ns3/antenna-model.h"
#include "ns3/mobility-model.h"
#include "ns3/sat-isl-net-device.h"
#include "ns3/sat-isl-channel.h"


namespace ns3
{


    typedef enum {
        RxOnly,
        TxOnly,
        RxTx
    } SatISLTerminalMode_t;



/**
 * \ingroup satellite
 *  
 * The Terminal represents a physical networking Interface, facing in a specific direction.
 * It holds Antenna Information and other information.
 *
 * \brief Physical ISL Terminal/Interface
 */
class SatelliteISLTerminal : public Object
{
public:

    typedef enum {
        PointToPoint,
        Dynamic
    } ISLTerminalType_t;


    static TypeId GetTypeId();
    TypeId GetInstanceTypeId() const;

    SatelliteISLTerminal();
    ~SatelliteISLTerminal();


    /**
     * @brief Set Reference Mobility Model of the Satellite (Parent)
     * 
     * @param mobility 
     */
    void SetParentMobility(Ptr<MobilityModel> mobility);

    Ptr<MobilityModel> GetParentMobility() const;

    /**
     * @brief Set the Terminal Orientation relative to the Parent Mobility Model.
     * 
     *        Where (0,0,0) means towards the origin of the Mobility Model
     * 
     * @param orientaton 
     */
    void SetRelativeOrientation(Angles orientaton);


    /**
     * @brief Get the Pointing direction of the Interface
     * 
     * @return Vector  Unit vector of the pointing direction
     */
    Vector GetOrientation() const;

    /**
     * @brief Set the Interface Antenna Model
     * 
     * @param antenna 
     */
    void SetAntennaModel(Ptr<AntennaModel> antenna);

    /**
     * @brief Attach the Terminal to a Channel
     * 
     * @param channel 
     */
    void AttachToChannel(Ptr<SatelliteISLChannel> channel);

    /**
     * @brief Check If the link to a specific node is available
     * 
     * @return true 
     * @return false 
     */
    bool IsLinkUp(Mac48Address dst) const;

    /**
     * @brief Tune the Transmitter to a Center Frequency
     * 
     * @param fc 
     */
    void SetCenterFrequency(double fc);


    /**
     * @brief Setup the Terminal to use an internal NetDevice
     * 
     * @param channel 
     * @param mobility 
     * 
     */
    void SetupInternalInterface(Ptr<SatelliteISLChannel> channel, Ptr<MobilityModel> mobility, Mac48Address address);

    /**
     * @brief Setup the Terminal to use an already setup, shared NetDevice
     * 
     * @param device 
     * 
     */
    void SetupSharedInterface(Ptr<SatelliteISLNetDevice> device);

    /**
     * @brief Check if the terminal is properly setup and ready
     * 
     * @return true 
     * @return false 
     */
    bool IsReady() const;

    Ptr<SatelliteISLNetDevice> GetNetDevice() const;

private:

    bool m_setup;

    bool m_updateOrientation;

    Ptr<SatelliteISLNetDevice> m_netitf;

    Ptr<MobilityModel> m_mobility;
    Ptr<SatelliteISLChannel> m_channel;


    SatISLTerminalMode_t m_phyMode;
    ISLTerminalType_t m_ttype;

    Angles m_orientation;

    bool m_sharedNetDevice;

    double      m_fc;
    double      m_dopplerBw;
    bool        m_dopplerMitigation;


    std::unordered_map<std::string, Ptr<SatelliteISLNetDevice>> m_nodes;


}; /* SatelliteISLTerminal */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_TERMINAL_H */