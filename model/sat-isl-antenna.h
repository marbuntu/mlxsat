/**
 * @brief   Satellite ISL Antenna Model
 * 
 * @file    sat-isl-antenna.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SATELLITE_ISL_ANTENNA_H
#define SATELLITE_ISL_ANTENNA_H


#include "ns3/antenna-model.h"
#include "ns3/random-variable-stream.h"


namespace ns3
{

    /**
     * @ingroup satellite
     * 
     * @brief Model of a basic Antenna for Inter-Satellite-Links
     * 
     *        The radiation Pattern is assumed to be rotationally symmetric arround the pointing vector of the main Lobe.
     *        
     */
    class SatelliteISLAntenna : public AntennaModel
    {
    public:

        typedef enum
        {
            RP_Cosine,
            RP_Bessel,
            RP_Constant
        } RadiationPattern_t;



        static TypeId GetTypeId();

        SatelliteISLAntenna();
        ~SatelliteISLAntenna();

        double GetGainDb(Angles a);


        void SetOpeningAngle(double angle);

        double GetOpeningAngle() const;


        double GetPointingErrorDb(const double gain_db) const;

        Ptr<RandomVariableStream> GetPointingErrorModel();

        // void SetAntennaOrientation(const Vector &vec);



    private:
        RadiationPattern_t m_pattern;
        double m_maxGainDbi;
        double m_openingAngle;

        Ptr<RandomVariableStream> m_errmodel;
        double m_errfactor;

    };  /* SatelliteISLAntenna */


};  /* namespace ns3 */


#endif /* SATELLITE_ISL_ANTENNA_H */