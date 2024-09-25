/**
 * @brief   Satellite ISL Terminal Helper
 * 
 * @file    sat-isl-terminal-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-09-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SAT_ISL_TERMINAL_HELPER_H
#define SAT_ISL_TERMINAL_HELPER_H


#include "ns3/object.h"
#include "ns3/object-factory.h"
#include "ns3/sat-isl-terminal.h"


namespace ns3
{

    class SatelliteISLTerminalHelper : public Object
    {
    public:
        
        static TypeId GetTypeId();
        
        SatelliteISLTerminalHelper();
        ~SatelliteISLTerminalHelper();


        //void SetRelativeOrientation(const double degree_phi, const double degree_theta, const double degree_psi);
        
        Ptr<SatelliteISLTerminal> Create() const;


        template <typename... Args>
        void SetTerminalConfig(Args... args)
        {
            m_terminalFactory.Set(args...);
        }


        template <class T, typename... Args>
        void SetAntennaModel(Args... args)
        {
            static_assert(std::is_base_of<Object, T>::value);
            m_antennaFactory.SetTypeId(T::GetTypeId().GetName());
            m_antennaFactory.Set(args...);
        }


    protected:

        ObjectFactory m_antennaFactory;
        ObjectFactory m_terminalFactory;
        double m_phi;
        double m_psi;
        double m_theta;


    }; /* SatelliteISLTerminalHelper */



};  /* namespace ns3 */


#endif /* SAT_ISL_TERMINAL_HELPER_H */