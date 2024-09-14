

#include <ns3/core-module.h>
#include <ns3/applications-module.h>

#include <ns3/vector-extensions.h>

#include "ns3/sat-isl-terminal.h"
#include "ns3/orientation-helper.h"
#include "ns3/cosine-antenna-model.h"

using namespace ns3;


NS_LOG_COMPONENT_DEFINE("Sat_Orientation_Helper_Demo");

/**
 * @brief  Programm demonstrating the ISL Orientation Calculations
 *         The Setup constists of two satellites. To demo the Orientation, one of them is 
 *         rotating. 
 * 
 */
int main( int argc, char* argv[] )
{
    LogComponentEnable("SatelliteISLTerminal", LOG_LEVEL_ALL);

    std::ofstream oss("./test-orientation.txt");
    oss << "idx\trotation\tv1\tazimuth\tinclination" << "\n";


    Vector p1(3, 0, 3);
    Vector p2(3, 0, 6);

    Ptr<CosineAntennaModel> ant = CreateObjectWithAttributes<CosineAntennaModel>(
        "MaxGain", DoubleValue(30.0),
        "VerticalBeamwidth", DoubleValue(120),
        "HorizontalBeamwidth", DoubleValue(45)
    );

    Ptr<SatelliteISLTerminal> tn = CreateObjectWithAttributes<SatelliteISLTerminal>();
    Ptr<SatelliteISLTerminal> te = CreateObjectWithAttributes<SatelliteISLTerminal>();
    Ptr<SatelliteISLTerminal> tw = CreateObjectWithAttributes<SatelliteISLTerminal>();
    Ptr<SatelliteISLTerminal> ts = CreateObjectWithAttributes<SatelliteISLTerminal>();

    Ptr<SatelliteISLTerminal> terms[] = {tn, te, tw, ts};


    LVLHReference ref = LVLHReference();
    

    tn->SetRelativeOrientation(0.0, 0.0, 0.0);
    tn->SetLocalReference(&ref);

    te->SetRelativeOrientation(0.0, 0.0, 90.0);
    te->SetLocalReference(&ref);

    ts->SetRelativeOrientation(0.0, 0.0, 180.0);
    ts->SetLocalReference(&ref);

    tw->SetRelativeOrientation(0.0, 0.0, 270.0);
    tw->SetLocalReference(&ref);

    size_t N = 80;
    double step = (2.0 / N) * M_PI;

    for (size_t n = 0; n < N; n++)
    {

        Vector v1(0, sin(n * step), cos(n * step));
        ref.UpdateLocalReference(p1, v1);

        oss << n << "\t" << n * step << "\t" << v1.x << "\t" << v1.y << "\t" << v1.z << "\t";
        for (const auto &t : terms)
        {
            Angles an = t->GetRelativeAngles(p2);
            oss << an.GetAzimuth() << "\t" << an.GetInclination() << "\t" << ant->GetGainDb(an) << "\t"; 
        }
        oss << "\n";

    }

    oss.close();

    return 0;
}
