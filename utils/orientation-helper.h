/**
 * @brief   Utils for Local Orientation
 * 
 * @file    orientation-helper.h
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef ORIENTATION_HELPER_H
#define ORIENTATION_HELPER_H

#include "ns3/object.h"
#include "ns3/vector.h"


namespace ns3
{
    /**
     * @brief   Calculate Unit Vector for vec
     * 
     * @param vec       Input Vector 
     * @return Vector   Unit Vector
     */
    Vector Normalize(const Vector &vec);


    /**
     * @brief   Wrap phi to [-pi, pi] and Convert to Rad
     * 
     * @param   phi     Angle in Degree 
     * @return  double  Angle in Rad
     */
    double ConvertPhi(const double phi);


    /**
     * @brief   Wrap Psi to [-pi, pi] and Convert to Rad
     * 
     * @param   psi     Angle in Degree 
     * @return  double  Angle in Rad
     */
    double ConvertPsi(const double psi);


    /**
     * @brief   Wrap theta to [-pi/2, pi/2] and Convert to Rad
     * 
     * @param   theta   Angle in Degree 
     * @return  double  Angle in Rad
     */
    double ConvertTheta(const double theta);



    /**
     * @brief   Helper Class to facilitate Quaternion operations
     * 
     */
    class Quaternion
    {
    public:

        Quaternion();
        ~Quaternion();


        /**
         * @brief   Construct Quaternion from Rotation Axis and Angle
         * 
         * @param   axis    Axis to rotate around 
         * @param   thetat  Angle to rotate 
         */
        Quaternion FromRotation(const Vector &axis, const double theta);


        Quaternion FromAngles(const double roll, const double pitch, const double yaw);


        Quaternion Inverse() const;


        /**
         * @brief   Rotate a Vector
         * 
         * @param   vec     Vector v to rotate
         * @return  Rotated Vector v'
         */
        
        Vector RotateVector(const Vector &vec) const;



        /**
         * @brief   Enable printable output
         * 
         * @param out 
         * @param quat 
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream& out, const Quaternion &quat);

        
        double w;   //! Scalar Component W
        double x;   //! Vector Component X
        double y;   //! Vector Component Y
        double z;   //! Vector Component Z

    private:


    };  /* Quaternion */


    class LVLHReference : public Object
    {
    public:

        static TypeId GetTypeId();

        LVLHReference();
        ~LVLHReference();


        /**
         * @brief Update Local LVLH-Coordinate Reference System
         * 
         * @param position      Position Vector
         * @param velocity      Velocity Vector, cannot be (0, 0, 0)
         */
        void UpdateLocalReference(const Vector &position, const Vector &velocity);


        Vector m_hr;        // z-Axis, Radial Component
        Vector m_ht;        // x-Axis, Tangential Component
        Vector m_hl;        // y-Axis, Orthorgonal Component
    };  /* LVLH Reference */



    class OrientationTransformationHelper
    {
    public:
        OrientationTransformationHelper();
        ~OrientationTransformationHelper();

        /**
         * @brief Set the Angles object
         * 
         * @param phi 
         * @param theta 
         * @param psi 
         */
        void SetAngles(const double phi, const double theta, const double psi);


        /**
         * @brief   Rotate around the x-axis by angle Phi
         * 
         * @param vec   Vector to Rotate
         * @param axis  x-Axis Unit Vector
         * @return Vector 
         */
        Vector Roll(const Vector &vec, const Vector &axis) const;
        

        /**
         * @brief   Rotate around the y-axis by angle Theta
         * 
         * @param vec   Vector to Rotate
         * @param axis  y-Axis Unit Vector
         * @return Vector 
         */
        Vector Pitch(const Vector &vec, const Vector &axis) const;


        /**
         * @brief   Rotate around the z-axis by angle Psi
         * 
         * @param vec   Vector to Rotate
         * @param axis  z-Axis Unit Vector
         * @return Vector 
         */
        Vector Yaw(const Vector &vec, const Vector &axis) const;


        Vector TransformVector(const Vector &vec, const LVLHReference &ref) const;

        Vector ReverseTransformVector(const Vector &vec, const LVLHReference &ref) const;

        // /**
        //  * @brief 
        //  * 
        //  * @param vec 
        //  * @return Vector 
        //  */
        // Vector RotateVector(const Vector &vec, ) const;

        //double CalcRelativeAngle(const Ptr<MobilitModel> &tx_mob, const Ptr<MobilityModel> &rx_mob) const;

    protected:

    private:


        double m_phi;       // Roll Angle
        double m_the;       // Pitch Angle
        double m_psi;       // Yaw Angle

    };  /* OrientationTransformationHelper */




};  /* namespace ns3 */


#endif /* ORIENTATION_HELPER_H */