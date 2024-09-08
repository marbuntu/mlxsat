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

#include "ns3/orientation-helper.h"


namespace ns3
{


typedef enum {
    RxOnly,
    TxOnly,
    RxTx
} SatISLTerminalMode_t;



class OrientationHelper
{
public:

    OrientationHelper();
    ~OrientationHelper();

    bool IsParallel(Vector vec1, Vector vec2, double tolerance) const;
    bool IsAntiParallel(Vector vec1, Vector vec2, double tolerance) const;

    bool UpdateOrientation(const Vector &position, const Vector &velocitiy);

    double CalculateOrientationAngle(const Vector &vec) const;

    Vector RadialComponent() const;
    Vector OrbitalComponent() const;
    Vector OrthorgonalComponent() const;

    // Vector Rotate(const Vector &vec) const;

//     Vector Rotate(const Vector &vec, const Quaternion_t &quat) const;

//     Quaternion_t RotationToQuaternion(const Vector &vec, double theta) const;
    
    Vector Normalized(const Vector &vec) const;

protected:


//     Vector RotateVector(const Vector &vec, const Quaternion_t &quat) const;
//     Quaternion_t RotationFromVectors(const Vector &v1, const Vector &v2) const;


private:

    double m_w;
    double m_x;
    double m_y;
    double m_z;

    Vector m_rot;

    Vector m_hr;
    Vector m_hl;
    Vector m_ht;


};  /* OrientationHelper */




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



    Ptr<SatelliteISLNetDevice> GetNetDevice() const;


private:

    bool m_setup;

    bool m_updateOrientation;

    /** The Netdevice has access to the channel and mobility model */
    Ptr<SatelliteISLNetDevice> m_netitf;

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



    Ptr<FriisPropagationLossModel> _getPropagationLossModel() const;
    Ptr<MobilityModel> _getMobilityModel() const;
    Ptr<Channel> _getChannelModel() const;


}; /* SatelliteISLTerminal */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_TERMINAL_H */