/**
 * @brief   HEADER TEMPLATE
 * 
 * @file    sat-fl-application-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_FL_APP_HELPER_H
#define SATELLITE_FL_APP_HELPER_H


#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "ns3/object-factory.h"
#include "ns3/sat-fl-application.h"


namespace ns3
{

    class SatFLApplicationHelper
    {
    public:

        SatFLApplicationHelper();


        void SetAttribute(std::string name, const AttributeValue &value);


        ApplicationContainer Install(NodeContainer c);


    protected:
    private:

        ObjectFactory m_factory;

    };  /* SFLApplicationHelper */


};  /* namespace ns3 */




#endif /* SATELLITE_FL_APP_HELPER_H */