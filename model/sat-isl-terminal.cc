/**
 * @brief   Inter-Satellite-Link Terminal
 * 
 * @file    sat-isl-terminal.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sat-isl-terminal.h"

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/vector-extensions.h"


namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("SatelliteISLTerminal");
    NS_OBJECT_ENSURE_REGISTERED(SatelliteISLTerminal);



    OrientationHelper::OrientationHelper()
    {
    }


    OrientationHelper::~OrientationHelper()
    {
    }


    bool OrientationHelper::UpdateOrientation(const Vector &position, const Vector &velocity)
    {
        NS_LOG_FUNCTION(this << position << velocity);



        return true;
    }


    Vector OrientationHelper::RadialComponent() const
    {
        return m_hr;
    }


    Vector OrientationHelper::OrthorgonalComponent() const
    {
        return m_hl;
    }


    Vector OrientationHelper::OrbitalComponent() const
    {
        return m_ht;
    }


    // Quaternion_t OrientationHelper::RotationToQuaternion(const Vector &vec, double theta) const
    // {
    //     NS_LOG_FUNCTION(this << vec << theta);

    //     Vector nrm = Normalized(vec);
    //     double half_theta = 0.5 * theta;

    //     Quaternion_t quat =
    //     {
    //         .x = nrm.x * sin(half_theta),
    //         .y = nrm.y * sin(half_theta),
    //         .z = nrm.z * sin(half_theta),
    //         .w = cos(half_theta)
    //     };

    //     return quat;
    // }


    // double OrientationHelper::CalculateOrientationAngle(const Vector &vec) const
    // {
    //     NS_LOG_FUNCTION(this << DotProduct(m_rot, vec) << vec);

    //     Vector vn = Normalized(vec);
    //     return acos(DotProduct(m_rot, vn));
    // }


    Vector OrientationHelper::Normalized(const Vector &vec) const
    {
        //return (vec * ( 1.0f / vec.GetLength()));
        double N = 1 / vec.GetLength();
        return Vector(vec.x * N, vec.y * N, vec.z * N);
    }


    // Vector OrientationHelper::RotateVector(const Vector& vec, const Quaternion_t &quat) const
    // {
    //     // Extract the vector part of the quaternion
    //     Vector u(quat.x, quat.y, quat.z);

    //     // Extract the scalar part of the quaternion
    //     double s = quat.w;

    //     // Do the math
    //     Vector vprime = 2.0 * DotProduct(u, vec) * u + ( s*s - DotProduct(u, u)) * vec + 2.0 * s * CrossProduct(u, vec);
        
    //     // 2.0f * DotProduct(u, vec) * u
    //     //    + (s*s - DotProduct(u, u)) * vec
    //     //    + 2.0f * s * CrossProduct(u, vec);

    //     return vprime;
    // }


    // Vector OrientationHelper::Rotate(const Vector &vec, const Quaternion_t &quat) const
    // {
    //     NS_LOG_FUNCTION(this << vec);

    //     return RotateVector(vec, quat);
    // }


    // Quaternion_t OrientationHelper::RotationFromVectors(const Vector &v1, const Vector &v2) const
    // {
    //     Quaternion_t quat = {.x = 0, .y = 0, .z = 0, .w = 0};

    //     Vector u1 = Normalized(v1);
    //     Vector u2 = Normalized(v2);

    //     Vector cross = CrossProduct(u1, u2);
    //     double dot = DotProduct(u1, u2);


    //     Vector axis = Normalized(cross);
    //     double half_theta = acos(dot) / 2.0;
    //     double sine_htheta = sin(half_theta);

    //     quat.x = axis.x * sine_htheta;
    //     quat.y = axis.y * sine_htheta;
    //     quat.z = axis.z * sine_htheta;
    //     quat.w = cos(half_theta);

    //     return quat;
    // }



    TypeId SatelliteISLTerminal::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::SatelliteISLTerminal")
            .SetParent<Object>()
            .AddConstructor<SatelliteISLTerminal>()
            .AddAttribute(
                "PhyMode"
                , "Sets the mode of the Terminal to  RxOnly, TxOnly or RxTx"
                , EnumValue(RxTx)
                , MakeEnumAccessor(&SatelliteISLTerminal::m_phyMode)
                , MakeEnumChecker(
                    RxOnly, "RxOnly",
                    TxOnly, "TxOnly",
                    RxTx, "RxTx"
                )
            )
            .AddAttribute(
                "ISLType"
                , "Set the ISL as Point-to-Point or Dynamic"
                , EnumValue(PointToPoint)
                , MakeEnumAccessor(&SatelliteISLTerminal::m_ttype)
                , MakeEnumChecker(
                    PointToPoint, "PointToPoint",
                    Dynamic, "Dynamic"
                )
            )
            .AddAttribute(
                "SharedNetDevice"
                , "Use one Shared Net Device (must be registered to instance),"
                  "else the Terminal will create an internal NetDevice."
                , BooleanValue(false)
                , MakeBooleanAccessor(&SatelliteISLTerminal::m_sharedNetDevice)
                , MakeBooleanChecker()
            )
            .AddAttribute(
                "DopplerBandwidth"
                , "Sets a Range for the center frequency to be tuned in order to mitigate Doppler Shift"
                  "while transmitting or the accaptable range of shift in a received signal."
                , DoubleValue(1e6)
                , MakeDoubleAccessor(&SatelliteISLTerminal::m_dopplerBw)
                , MakeDoubleChecker<double>(0.0)
            )
            .AddAttribute(
                "DopplerMitigation"
                , "Tunes the center Frequency at the transmitter to match the intended Frequency at the receiver."
                  "The max Range is controlled by the Doppler Bandwidth Attribute."
                , BooleanValue(true)
                , MakeBooleanAccessor(&SatelliteISLTerminal::m_dopplerMitigation)
                , MakeBooleanChecker()
            )
            .AddAttribute(
                "UpdateOrientation"
                , "Make the Orientation of the Interface dependent of the direction of movement of the spacecraft."
                  "If set to false, the initialy set orientation will not change during the Simulation."
                , BooleanValue(true)
                , MakeBooleanAccessor(&SatelliteISLTerminal::m_updateOrientation)
                , MakeBooleanChecker()
            )
        ;

        return tid;
    }


    TypeId SatelliteISLTerminal::GetInstanceTypeId() const
    {
        return GetTypeId();
    }


    SatelliteISLTerminal::SatelliteISLTerminal()
    : m_orientation(Quaternion())
    {
    }


    SatelliteISLTerminal::~SatelliteISLTerminal()
    {
    }


    Ptr<MobilityModel> SatelliteISLTerminal::GetParentMobility() const
    {
        NS_LOG_FUNCTION(this);
        return m_netitf->GetObject<MobilityModel>();
    }


    // void SatelliteISLTerminal::SetRelativeOrientation(Angles orientation)
    // {
    //     NS_LOG_FUNCTION(this << orientation);
    //     //m_orientation = orientation;
    // }


    // void SatelliteISLTerminal::SetRelativeOrientation(const Vector &orientation)
    // {
    //     NS_LOG_FUNCTION(this << orientation);


    //     //m_orientation = quat;
    // }


    void SatelliteISLTerminal::SetLocalReference(Ptr<LVLHReference> ref)
    {
        NS_LOG_FUNCTION(this << ref);
        m_ref = ref;
    }


    void SatelliteISLTerminal::SetRelativeOrientation(const double degree_phi, const double degree_theta, const double degree_psi)
    {
        NS_LOG_FUNCTION(this << degree_phi << degree_theta << degree_psi);

        m_pointingHelper.SetAngles(degree_phi, degree_theta, degree_psi);
    }    



    Vector SatelliteISLTerminal::GetOrientation() const
    {
        
        Vector nullvec = Vector(0, 0, 0);

        if (m_updateOrientation)
        {
            Vector sc = m_netitf->GetObject<MobilityModel>()->GetVelocity();

            if (sc.GetLength() == 0) return nullvec;

            NS_LOG_FUNCTION(this << sc);

            

        }


        return nullvec;
    }


    void SatelliteISLTerminal::SetCenterFrequency(double fc)
    {
        NS_LOG_FUNCTION(this << fc);
        m_fc = fc;
    }


    bool SatelliteISLTerminal::IsLinkUp(Mac48Address dst) const
    {
        NS_LOG_FUNCTION(this << dst);


        // Check if the Device is attached to the same Channel
        Ptr<SatelliteISLChannel> chn = StaticCast<SatelliteISLChannel>(m_netitf->GetChannel());
        Ptr<NetDevice> dev = chn->GetDevice(dst);

        Ptr<MobilityModel> mob_rx = dev->GetObject<MobilityModel>();

        double dist = m_netitf->GetObject<MobilityModel>()->GetDistanceFrom(mob_rx);

        NS_LOG_UNCOND(dist);

        if (dev == nullptr) return false;

        GetOrientation();

        if (m_ttype == PointToPoint) return true;

        // \todo Calculate the direction, distance and received power to see of the link is available.


        return false;
    }


    bool SatelliteISLTerminal::IsReady() const
    {
        return m_setup;
    }


    void SatelliteISLTerminal::SetupInternalInterface(Ptr<SatelliteISLChannel> channel, Ptr<MobilityModel> mobility, Mac48Address address)
    {
        NS_LOG_FUNCTION(this << channel << mobility << address);

        NS_ASSERT_MSG(channel, "Parameter channel is nullptr");
        NS_ASSERT_MSG(mobility, "Parameter mobility is nullptr");

        m_netitf = CreateObject<SatelliteISLNetDevice>();
        m_netitf->SetAddress(address);
        m_netitf->SetChannel(channel);
        m_netitf->AggregateObject(mobility);

        m_setup = true;
    }


    void SatelliteISLTerminal::SetupSharedInterface(Ptr<SatelliteISLNetDevice> device)
    {
        NS_LOG_FUNCTION(this << device);

        NS_ASSERT_MSG(device != nullptr, "Parameter is nullptr");
        NS_ASSERT_MSG(device->GetChannel(), "Cannot assign NetDevice without aggregated Channel!");
        NS_ASSERT_MSG(device->GetObject<MobilityModel>(), "Cannot assign NetDevice without aggregated Mobility Model!");
        
        m_netitf = device;
    }


    Ptr<SatelliteISLNetDevice> SatelliteISLTerminal::GetNetDevice() const
    {
        return m_netitf;
    }


    void SatelliteISLTerminal::SetAntennaModel(Ptr<AntennaModel> antenna)
    {
        NS_LOG_FUNCTION(this << antenna);
        m_antenna = antenna;
    }


    Vector toUnitVector(Vector vec)
    {
        double len = vec.GetLength();
        if (len != 0) {
            return Vector(vec.x / len, vec.y / len, vec.z / len);
        }

        return Vector(0, 0, 0);
    }

    void SatelliteISLTerminal::test(Ptr<SatelliteISLTerminal> other)
    {
        NS_LOG_FUNCTION(this << other);

        Vector v = toUnitVector(this->GetParentMobility()->GetVelocity());
        Vector p = toUnitVector(this->GetParentMobility()->GetPosition());

        Angles ori = Angles(v, p);
        Angles dir = Angles(other->GetParentMobility()->GetPosition(), this->GetParentMobility()->GetPosition());

        NS_LOG_INFO(RadiansToDegrees(dir.GetInclination()));
        NS_LOG_INFO(RadiansToDegrees(ori.GetAzimuth()) << " " << RadiansToDegrees(ori.GetInclination()));


        Ptr<SatelliteISLChannel> chn = StaticCast<SatelliteISLChannel>(m_netitf->GetChannel());

        double Ga = m_antenna->GetGainDb(dir);

        double fspl = chn->GetPropagationLossModel()->CalcRxPower(0.0, this->GetParentMobility(), other->GetParentMobility());

        printf("FSPL: %.04f \t Ga: %.04f\n", fspl, Ga);


        NS_LOG_INFO("\n");

    }


    double SatelliteISLTerminal::GetRelativeAngle(const Vector &vec) const
    {
        NS_LOG_FUNCTION(this << vec);

        Vector ptd = m_pointingHelper.TransformVector(m_ref->m_ht, *m_ref);
        double cp = acos(DotProduct(ptd, Normalize(vec)));

        NS_LOG_FUNCTION(this << ptd << cp);

        return 0.0;
    }


    double SatelliteISLTerminal::GetRelativeAngleDeg(const Vector &vec) const
    {
        return RadiansToDegrees(GetRelativeAngle(vec));
    }


}   /* namespace ns3 */