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
#include "ns3/mac48-address.h"
#include "ns3/antenna-model.h"
#include "ns3/mobility-model.h"
#include "ns3/orientation-helper.h"
#include "ns3/data-rate.h"
#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/propagation-loss-model.h"


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


    // /**
    //  * @brief Set Reference Mobility Model of the Satellite (Parent)
    //  * 
    //  * @param mobility 
    //  */
    // void SetParentMobility(Ptr<MobilityModel> mobility);


    Ptr<MobilityModel> GetParentMobility() const;


    // /**
    //  * @brief Set the Terminal Orientation relative to the Parent Mobility Model.
    //  * 
    //  *        Where (0,0,0) means towards the origin of the Mobility Model
    //  * 
    //  * @param orientaton 
    //  */
    // void SetRelativeOrientation(Angles orientaton);



    // void SetRelativeOrientation(const Vector &orientation);

    /**
     * @brief   Register Local Reference Helper
     * 
     * @param ref 
     */
    void SetLocalReference(Ptr<LVLHReference> ref);


    /**
     * @brief Set the Relative Orientation in Tait-Bryan Angles
     * 
     * @param phi       Roll Angle (around x) within [-pi, pi]
     * @param theta     Pitch Angle (around y) within [-pi/2, pi/2]
     * @param psi       Yaw Angle (around z) within [-pi, pi]
     */
    void SetRelativeOrientation(const double degree_phi, const double degree_theta, const double degree_psi);


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


    // /**
    //  * @brief Attach the Terminal to a Channel
    //  * 
    //  * @param channel 
    //  */
    // void AttachToChannel(Ptr<SatelliteISLChannel> channel);


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


    // /**
    //  * @brief Setup the Terminal to use an internal NetDevice
    //  * 
    //  * @param channel 
    //  * @param mobility 
    //  * 
    //  */
    // void SetupInternalInterface(Ptr<SatelliteISLChannel> channel, Ptr<MobilityModel> mobility, Mac48Address address);


    // /**
    //  * @brief Setup the Terminal to use an already setup, shared NetDevice
    //  * 
    //  * @param device 
    //  * 
    //  */
    // void SetupSharedInterface(Ptr<SatelliteISLNetDevice> device);


    /**
     * @brief Check if the terminal is properly setup and ready
     * 
     * @return true 
     * @return false 
     */
    bool IsReady() const;


    /**
     * @brief Get Azimuth and Inclination Angles for a Satellite Position
     *        relative to the Terminal Orientation
     * 
     *        The Angles can be used with AntennaModel
     * 
     * @param satpos    Other Satellite Position
     * @return Angles   (Azimuth, Inclination)
     */
    Angles GetRelativeAngles(const Vector &satpos) const;



    DataRate GetRateEstimation(const Ptr<MobilityModel> self, Ptr<MobilityModel> other, const Ptr<PropagationLossModel> loss) const;

    /**
     * @brief   Transmit Packet to other Satellite
     * 
     * @param pck 
     * @param src 
     * @param dst 
     * @param chn 
     * @return Time     TxTime 
     */
    Time Transmit(Ptr<Packet> pck,Ptr<NetDevice> src, Ptr<NetDevice> dst, Ptr<Channel> chn);


    // Ptr<SatelliteISLNetDevice> GetNetDevice() const;


private:

    bool m_setup;

    bool m_updateOrientation;

    /** The Netdevice has access to the channel and mobility model */
    //Ptr<SatelliteISLNetDevice> m_netitf;

    // Ptr<MobilityModel> m_mobility;
    // Ptr<SatelliteISLChannel> m_channel;
    Ptr<AntennaModel> m_antenna;

    //Ptr<OrientationHelper> m_orient;


    SatISLTerminalMode_t m_phyMode;
    ISLTerminalType_t m_ttype;

    //! Relative Antenna Orientation
    OrientationTransformationHelper m_pointingHelper;
    Ptr<LVLHReference>  m_ref;
    Quaternion m_orientation;

    bool m_sharedNetDevice;

    double      m_fc;
    double      m_dopplerBw;
    bool        m_dopplerMitigation;


    Ptr<PropagationLossModel> m_lossModel;



    // Ptr<FriisPropagationLossModel> _getPropagationLossModel() const;
    // Ptr<MobilityModel> _getMobilityModel() const;
    // Ptr<Channel> _getChannelModel() const;


}; /* SatelliteISLTerminal */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_TERMINAL_H */