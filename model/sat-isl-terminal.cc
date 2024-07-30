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
        ;

        return tid;
    }


    TypeId SatelliteISLTerminal::GetInstanceTypeId() const
    {
        return GetTypeId();
    }


    SatelliteISLTerminal::SatelliteISLTerminal()
    : m_orientation(Vector(0, 0, 0))
    {
    }


    SatelliteISLTerminal::~SatelliteISLTerminal()
    {
    }


    void SatelliteISLTerminal::AttachToChannel(Ptr<SatelliteISLChannel> channel)
    {
        NS_LOG_FUNCTION(this << channel);
        m_channel = channel;
    }


    void SatelliteISLTerminal::SetRelativeOrientation(Angles orientation)
    {
        NS_LOG_FUNCTION(this << orientation);
        m_orientation = orientation;
    }


    void SatelliteISLTerminal::SetNetDevice(Ptr<SatelliteISLNetDevice> device)
    {
        NS_LOG_FUNCTION(this << device);
        m_netdevice = device;
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

        Ptr<NetDevice> dev = m_channel->GetDevice(dst);
        if (dev == nullptr) return false;

        if (m_ttype == PointToPoint) return true;

        // \todo Calculate the direction, distance and received power to see of the link is available.


        return false;
    }


}   /* namespace ns3 */