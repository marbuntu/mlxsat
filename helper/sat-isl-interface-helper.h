/**
 * @brief   Helper to setup Satellite ISL Interfaces
 * 
 * @file    sat-isl-interface-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-09-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SAT_ISL_INTERFACE_HELPER_H
#define SAT_ISL_INTERFACE_HELPER_H

#include "ns3/object.h"
#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/sat-isl-channel.h"
#include "ns3/sat-isl-terminal-helper.h"
#include "ns3/sat-isl-net-device.h"

namespace ns3
{


//template <class T>
//void SetTerminalAntennaTypeId(std::string typeId);


class SatelliteISLInterfaceHelper : public Object
{
public:

    static TypeId GetTypeId();

    SatelliteISLInterfaceHelper();
    ~SatelliteISLInterfaceHelper();


    Ptr<SatelliteISLNetDevice> Create() const;

    Ptr<SatelliteISLNetDevice> CreateAndAggregate(Ptr<Node> node, const Ptr<SatelliteISLChannel> channel) const;

    template <typename... Args>
    Ptr<SatelliteISLTerminalHelper> AddTerminal(Args... args)
    {
        Ptr<SatelliteISLTerminalHelper> fac = CreateObjectWithAttributes<SatelliteISLTerminalHelper>(args...);
        m_terminalFactories.insert(m_terminalFactories.end(), fac);

        return fac;
    }


private:
    std::vector<Ptr<SatelliteISLTerminalHelper>> m_terminalFactories;


};  /* SatelliteISLInterfaceHelper */



class DefaultISLInterfaceSetup
{
public:
    typedef enum {
        SYMMETRIC_4x = 0,
        ASYMMETRIC_2x2 = 1
    } setup_t;


    static SatelliteISLInterfaceHelper GetDefaultFactory(setup_t setup);


protected:
    DefaultISLInterfaceSetup() {};
    ~DefaultISLInterfaceSetup() {};

    /**
     * @brief 4 Terminals in N-S-E-W direction, Shared Antenna Characteristics
     * 
     * @return SatelliteISLInterfaceHelper 
     */
    static SatelliteISLInterfaceHelper _Symmetric4x();

    /**
     * @brief 2x2 Terminal Paris in N-S and E-W direction. Pairs with shared Antenna Characteristics.
     * 
     * @return SatelliteISLInterfaceHelper 
     */
    static SatelliteISLInterfaceHelper _Asymmetric2x2();

};  /* DefaultISLInterfaceSetup */


};  /* namespace ns3 */


#endif /* SAT_ISL_INTERFACE_HELPER_H */