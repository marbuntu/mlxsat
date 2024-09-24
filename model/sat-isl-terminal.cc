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
#include "ns3/node.h"
#include "ns3/vector-extensions.h"
#include "ns3/sat-isl-pck-tag.h"
#include "ns3/sat-isl-channel.h"
#include "ns3/satellite-const-variables.h"

namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("SatelliteISLTerminal");
    NS_OBJECT_ENSURE_REGISTERED(SatelliteISLTerminal);



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


    // Ptr<MobilityModel> SatelliteISLTerminal::GetParentMobility() const
    // {
    //     NS_LOG_FUNCTION(this);
    //     return m_netitf->GetObject<MobilityModel>();
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



    // Vector SatelliteISLTerminal::GetOrientation() const
    // {
        
    //     Vector nullvec = Vector(0, 0, 0);

    //     if (m_updateOrientation)
    //     {
    //         Vector sc = m_netitf->GetObject<MobilityModel>()->GetVelocity();

    //         if (sc.GetLength() == 0) return nullvec;

    //         NS_LOG_FUNCTION(this << sc);
    //     }

    //     return nullvec;
    // }


    void SatelliteISLTerminal::SetCenterFrequency(double fc)
    {
        NS_LOG_FUNCTION(this << fc);
        m_fc = fc;
    }


    // bool SatelliteISLTerminal::IsLinkUp(Mac48Address dst) const
    // {
    //     NS_LOG_FUNCTION(this << dst);


    //     // Check if the Device is attached to the same Channel
    //     Ptr<SatelliteISLChannel> chn = StaticCast<SatelliteISLChannel>(m_netitf->GetChannel());
    //     Ptr<NetDevice> dev = chn->GetDevice(dst);

    //     Ptr<MobilityModel> mob_rx = dev->GetObject<MobilityModel>();

    //     double dist = m_netitf->GetObject<MobilityModel>()->GetDistanceFrom(mob_rx);

    //     NS_LOG_UNCOND(dist);

    //     if (dev == nullptr) return false;

    //     GetOrientation();

    //     if (m_ttype == PointToPoint) return true;

    //     // \todo Calculate the direction, distance and received power to see of the link is available.


    //     return false;
    // }


    bool SatelliteISLTerminal::IsReady() const
    {
        return m_setup;
    }


    // void SatelliteISLTerminal::SetupInternalInterface(Ptr<SatelliteISLChannel> channel, Ptr<MobilityModel> mobility, Mac48Address address)
    // {
    //     NS_LOG_FUNCTION(this << channel << mobility << address);

    //     NS_ASSERT_MSG(channel, "Parameter channel is nullptr");
    //     NS_ASSERT_MSG(mobility, "Parameter mobility is nullptr");

    //     m_netitf = CreateObject<SatelliteISLNetDevice>();
    //     m_netitf->SetAddress(address);
    //     m_netitf->SetChannel(channel);
    //     m_netitf->AggregateObject(mobility);

    //     m_setup = true;
    // }


    // void SatelliteISLTerminal::SetupSharedInterface(Ptr<SatelliteISLNetDevice> device)
    // {
    //     NS_LOG_FUNCTION(this << device);

    //     NS_ASSERT_MSG(device != nullptr, "Parameter is nullptr");
    //     NS_ASSERT_MSG(device->GetChannel(), "Cannot assign NetDevice without aggregated Channel!");
    //     NS_ASSERT_MSG(device->GetObject<MobilityModel>(), "Cannot assign NetDevice without aggregated Mobility Model!");
        
    //     m_netitf = device;
    // }


    // Ptr<SatelliteISLNetDevice> SatelliteISLTerminal::GetNetDevice() const
    // {
    //     return m_netitf;
    // }


    void SatelliteISLTerminal::SetAntennaModel(Ptr<AntennaModel> antenna)
    {
        NS_LOG_FUNCTION(this << antenna);
        m_antenna = antenna;
    }


    Ptr<AntennaModel> SatelliteISLTerminal::GetAntennaModel() const
    {
        return m_antenna;
    }


    Vector toUnitVector(Vector vec)
    {
        double len = vec.GetLength();
        if (len != 0) {
            return Vector(vec.x / len, vec.y / len, vec.z / len);
        }

        return Vector(0, 0, 0);
    }


    Angles SatelliteISLTerminal::GetRelativeAngles(const Vector &satpos) const
    {
        NS_LOG_FUNCTION(this << satpos << m_ref->m_hr << m_ref->m_ht << m_ref->m_hl);

        // NS_LOG_FUNCTION(this << m_ref->m_t1);
        // NS_LOG_FUNCTION(this << m_ref->m_t2);

        Vector ptd = m_pointingHelper.TransformVector(m_ref->m_ht, *m_ref);

        Vector rev = m_ref->ToWorldSpace(satpos); //m_pointingHelper.ReverseTransformVector(vec, *m_ref);

        NS_LOG_FUNCTION(this << rev);
        rev = m_pointingHelper.TransformVector(rev, *m_ref);
        Angles ang = Angles(rev);

        // NS_LOG_FUNCTION(this << ptd);
        // NS_LOG_FUNCTION(this << rev);
        //NS_LOG_FUNCTION(this << "Azi: " << RadiansToDegrees(ang.GetAzimuth()) << " Inc: " << RadiansToDegrees(ang.GetInclination()));

        return ang;
    }

    double SatelliteISLTerminal::GetAntennaGain(const Ptr<MobilityModel> self, Ptr<MobilityModel> other) const
    {
        // Vector rev = m_ref->ToWorldSpace(other->GetPosition());

        // NS_LOG_FUNCTION(this << rev);

        Angles ant_angles = GetRelativeAngles(other->GetPosition());

        double ant_gain = m_antenna->GetGainDb(ant_angles);
    
        return ant_gain;
    }


    DataRate SatelliteISLTerminal::GetRateEstimation(const Ptr<MobilityModel> self, Ptr<MobilityModel> other, const Ptr<PropagationLossModel> loss) const
    {
        double fc = 40e9;
        double B = fc * 0.02;


        Ptr<FriisPropagationLossModel> lossm = StaticCast<FriisPropagationLossModel>(loss);

        lossm->SetFrequency(fc);
        double loss_dbm = lossm->CalcRxPower(45, self, other);

        Angles ant_angles = GetRelativeAngles(other->GetPosition());

        double ant_gain = m_antenna->GetGainDb(ant_angles);
        

        double kBT = SatConstVariables::BOLTZMANN_CONSTANT * 1000.0 * B;

        double S = std::pow(10.0, 0.1 * (((loss_dbm + ant_gain)))); //- 30.0)));

        NS_LOG_FUNCTION(this << loss_dbm << "\t" << ant_gain << "\t" << 10 * std::log10(S / kBT));

        uint64_t rate =(uint64_t)  std::floor(B * log2(1 + (S / kBT)));

        NS_LOG_FUNCTION(this << S << " sig " << rate << " bps");


        return DataRate(rate);
    }



    Time SatelliteISLTerminal::Transmit(Ptr<Packet> pck,Ptr<NetDevice> src_dev, Ptr<NetDevice> other, Ptr<Channel> chn)
    {
        NS_LOG_FUNCTION(this << pck << other);

        Ptr<SatelliteISLChannel> sat_chn = StaticCast<SatelliteISLChannel>(chn);

        Ptr<MobilityModel> other_mob = other->GetNode()->GetObject<MobilityModel>();
        Ptr<MobilityModel> self_mob = src_dev->GetNode()->GetObject<MobilityModel>();

        if (other_mob == nullptr || self_mob == nullptr)
        {
            NS_LOG_FUNCTION(this << "Critical Error, No Mobility model associated!");
            return Time(0);
        }

        Ptr<PropagationLossModel> loss = sat_chn->GetPropagationLossModel();

        DataRate dr = GetRateEstimation(self_mob, other_mob, loss);

        if (dr.GetBitRate() <= 0)
        {
            return Time(0);
        }

        ISLPacketTag tag;
        pck->RemovePacketTag(tag);

        Mac48Address src = tag.GetSrc();
        Mac48Address dst = tag.GetDst();
        uint16_t proto = tag.GetProto();

        sat_chn->Send(pck, proto, dst, src, src_dev);

        Time txTime = dr.CalculateBytesTxTime(pck->GetSize());

        txTime.GetMilliSeconds();

        return txTime; 
    }


}   /* namespace ns3 */