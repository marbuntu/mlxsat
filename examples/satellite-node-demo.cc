


#include <ns3/core-module.h>
#include <ns3/applications-module.h>

#include "ns3/sat-node.h"
using namespace ns3;


NS_LOG_COMPONENT_DEFINE("Satellite_Node_Demo");



/**
 * @brief  Programm demonstrating the Satellite Node Class 
 * 
 */
int main( int argc, char* argv[] )
{
    //LogComponentEnable("SatelliteNode", LOG_LEVEL_ALL);



    Ptr<SatelliteISLTerminal> ter = CreateObjectWithAttributes<SatelliteISLTerminal>();
    //ter->SetupSharedInterface();

    Ptr<SatelliteNodeHelper> sat = CreateObject<SatelliteNodeHelper>();

    printf("Terminal %ld\n", sat->AddISLTerminal(std::string("TE"), ter));


    




    return 0;
}
