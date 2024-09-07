

#include <ns3/core-module.h>
#include <ns3/applications-module.h>

#include <ns3/vector-extensions.h>

#include "ns3/sat-isl-terminal.h"
#include "ns3/orientation-helper.h"

using namespace ns3;


NS_LOG_COMPONENT_DEFINE("Sat_Orientation_Helper_Demo");


int main( int argc, char* argv[] )
{
    LogComponentEnable("SatelliteISLTerminal", LOG_LEVEL_ALL);

    Vector p1(2, 0, 3);
    Vector p2(0, 3, 0);

    Vector v1(-1, 0, 0);
    Vector v2(-1, 0, 0);


    Ptr<SatelliteISLTerminal> ter = CreateObjectWithAttributes<SatelliteISLTerminal>();

    OrientationTransformationHelper orien = OrientationTransformationHelper();
    orien.SetAngles(0, 0, -90.0);

    LVLHReference ref = LVLHReference();
    ref.UpdateLocalReference(p1, v1);

    ter->SetRelativeOrientation(0, 0, -90.0);
    ter->SetLocalReference(&ref);

    //Vector axis = orien.RadialComponent();
    //Vector dir = orien.OrbitalComponent();
    //Quaternion quat = Quaternion().FromRotation(axis, -0.5 * M_PI);



    Vector diff = Normalize(Vector(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z));


    // Vector qr_v = orien.Rotate(dir, rot);

    // printf("\n QT: %.04f %.04f %.04f %.04f \n", rot.w, rot.x, rot.y, rot.z);
    // printf(" VR: %.04f %.04f %.04f \n\n", qr_v.x, qr_v.y, qr_v.z);

    // double AoD = acos(DotProduct(qr_v, diff));
    // printf("\nAoD: %f rad / %.4f °\r\n", AoD, AoD * (180.0 / M_PI));


    Vector qr_v = orien.TransformVector(ref.m_ht, ref);// quat.RotateVector(dir);

    ter->GetRelativeAngle(diff);

    //printf("\n QT: %.04f %.04f %.04f %.04f \n", rot.w, rot.x, rot.y, rot.z);
    printf(" VR: %.04f %.04f %.04f \n\n", qr_v.x, qr_v.y, qr_v.z);

    double AoD = acos(DotProduct(qr_v, diff));
    printf("\nAoD: %f rad / %.4f °\r\n", AoD, AoD * (180.0 / M_PI));


    SatelliteISLTerminal terminal;
    terminal.SetRelativeOrientation(0.0, 0.0, -90.0);

    //terminal.SetRelativeOrientation(quat);



    // //Quaternion_t OE = { .w = 0, .x = 0, .y = -0.5 * M_PI };


    // Vector p2_nrm = orien.Normalized(p2); //Vector(p2.x / p2.GetLength(), p2.y / p2.GetLength(), p2.z / p2.GetLength());
    // Vector di_nrm = orien.Normalized(diff);

    // double AoA = orien.CalculateOrientationAngle(diff);
    // double AoD = (DotProduct(qr_v, orien.Normalized(diff)));

    // printf("\r\ndiff: %.03f %.03f %.03f \r\n", diff.x, diff.y, diff.z);

    // printf("\nAoA: %.4f rad / %.4f °\r\n", AoA, AoA * (180.0 / M_PI));

    //orien.Rotate(p1);


    return 0;
}