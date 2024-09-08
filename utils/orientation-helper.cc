/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    _template.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "orientation-helper.h"
#include "ns3/vector-extensions.h"
#include "ns3/angles.h"



namespace ns3
{


    Vector Normalize(const Vector &vec)
    {
        double N = 1 / vec.GetLength();
        return Vector(vec.x * N, vec.y * N, vec.z * N);
    }


    double ConvertPhi(const double phi)
    {
        return DegreesToRadians(WrapTo180(phi));
    }


    double ConvertPsi(const double psi)
    {
       return DegreesToRadians(WrapTo180(psi));
    }


    double ConvertTheta(const double theta)
    {
        double a = WrapTo180(theta);
        printf("%.02f\n",a);
        if (a < -90)
        {
            a = -180 - a;
        }


        NS_ASSERT_MSG(-90 <= a && a <= 90, "Invalid wrap to [-90, 90[ / a=" << a);

        return DegreesToRadians(a);
    }



//  BEGIN: Quaternion +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    Quaternion::Quaternion()
    {
    }


    Quaternion::~Quaternion()
    {
    }


    Quaternion Quaternion::FromRotation(const Vector &vec, const double theta)
    {
        //NS_LOG_FUNCTION(this << vec << theta);

        Vector nrm = Normalize(vec);
        double half_theta = 0.5 * theta;

        this->x = nrm.x * sin(half_theta);
        this->y = nrm.y * sin(half_theta);
        this->z = nrm.z * sin(half_theta);
        this->w = cos(half_theta);

        return *this;
    }


    Quaternion Quaternion::FromAngles(const double roll, const double pitch, const double yaw)
    {
        double cr = cos(roll * 0.5);
        double sr = sin(roll * 0.5);
        double cp = cos(pitch * 0.5);
        double sp = sin(pitch * 0.5);
        double cy = cos(yaw * 0.5);
        double sy = sin(yaw * 0.5);

        w = cr * cp * cy + sr * sp * sy;
        x = sr * cp * cy - cr * sp * sy;
        y = cr * sp * cy + sr * cp * sy;
        z = cr * cp * sy - sr * sp * cy;

        return *this;
    }


    Quaternion Quaternion::FromVectors(const Vector &v, const Vector &v_prime)
    {
        Vector ev = Normalize(v);
        Vector ep = Normalize(v_prime);

        Vector axis = Normalize(CrossProduct(ev, ep));
        double theta = 0.5 * acos(DotProduct(ev, ep));


        if (isnan(axis.x) || isnan(axis.y) || isnan(axis.z))
        {
            this->w = 0.0;
            this->x = 0.0;
            this->y = 0.0;
            this->z = 0.0;

            return *this;
        }


        printf("AXIS: %.02f %.02f %.02f\n", axis.x, axis.y, axis.z);

        this->w = cos(theta);
        this->x = axis.x * sin(theta);
        this->y = axis.y * sin(theta);
        this->z = axis.z * sin(theta);

        return *this;
    }


    Quaternion Quaternion::Inverse() const
    {
        Quaternion nq;
        nq.w = this->w;
        nq.x = -this->x;
        nq.y = -this->y;
        nq.z = -this->z;

        return nq;
    }


    Vector Quaternion::RotateVector(const Vector &vec) const
    {
        // Extract the vector part of the quaternion
        Vector u(this->x, this->y, this->z);

        // Extract the scalar part of the quaternion
        double s = this->w;

        // Do the math
        Vector vprime = 2.0 * DotProduct(u, vec) * u + ( s*s - DotProduct(u, u)) * vec + 2.0 * s * CrossProduct(u, vec);


        return vprime;
    }


    double Quaternion::Norm() const
    {
        return sqrt( w*w + x*x + y*y + z*z );
    }


    std::ostream& operator<<(std::ostream& out, const Quaternion &quat)
    {
        return out << "( w: " << quat.w << " , x: " << quat.x << ", y: " << quat.y << ", z: " << quat.z << " )"; 
    }


    TypeId LVLHReference::GetTypeId()
    {
        static TypeId tid = TypeId()
            .SetParent<Object>()
            .AddConstructor<LVLHReference>()
        ;
        return tid;
    }


//  BEGIN: LVLHReference +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    LVLHReference::LVLHReference()
    : m_ex(1, 0, 0)
    , m_ey(0, 1, 0)
    , m_ez(0, 0, 1)
    {
    }


    LVLHReference::~LVLHReference()
    {
    }


    void LVLHReference::UpdateLocalReference(const Vector &position, const Vector &velocity)
    {
        NS_ASSERT_MSG(velocity.GetLength() > 0, "The Velocity Vector cannot be (0, 0, 0)!");

        if (position.GetLength() == 0)
        {
            m_hr.x = 0;
            m_hr.y = 0;
            m_hr.z = 1;
            m_origin = Vector(0, 0, 0);
        }
        else
        {
            m_hr = Normalize(position);
            m_origin = position;
        }

        m_hl = Normalize(CrossProduct(position, velocity));
        m_ht = Normalize(CrossProduct(m_hl, m_hr));


        m_t1 = Quaternion().FromVectors(m_hr, m_ez);

        if (m_t1.Norm() == 0)
        {
            m_t1 = Quaternion().FromVectors(m_ht, m_ex);
        }
        else
        {
            Vector ht_p = m_t1.RotateVector(m_ht);
            m_t2 = Quaternion().FromVectors(ht_p, m_ex);
        }
    }


    Vector LVLHReference::ToWorldSpace(const Vector &vec) const
    {
        Vector vp = vec - m_origin;

        if (m_t1.Norm() == 0) return vp;
        vp = m_t1.RotateVector(vp);

        if (m_t2.Norm() == 0) return vp;
        return m_t2.RotateVector(vp);
    }



//  BEGIN: OrientationTransformationHelper +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    OrientationTransformationHelper::OrientationTransformationHelper()
    {
    }


    OrientationTransformationHelper::~OrientationTransformationHelper()
    {
    }


    void OrientationTransformationHelper::SetAngles(const double phi, const double theta, const double psi)
    {
        m_phi = ConvertPhi(phi);
        m_psi = ConvertPsi(psi);
        m_the = ConvertTheta(theta);
    }


    

    
    Vector OrientationTransformationHelper::Yaw(const Vector &vec, const Vector &axis) const
    {
        Quaternion quat = Quaternion().FromRotation(axis, m_psi);
        return quat.RotateVector(vec);
    }


    Vector OrientationTransformationHelper::Pitch(const Vector &vec, const Vector &axis) const
    {
        Quaternion quat = Quaternion().FromRotation(axis, m_the);
        return quat.RotateVector(vec);
    }


    Vector OrientationTransformationHelper::Roll(const Vector &vec, const Vector &axis) const
    {
        Quaternion quat = Quaternion().FromRotation(axis, m_phi);
        return quat.RotateVector(vec);
    }


    Vector OrientationTransformationHelper::TransformVector(const Vector &vec, const LVLHReference &ref) const
    {
        //Vector imd = vec;

        printf("\nTrans: %.02f %.02f %.02f\n", m_phi, m_the, m_psi);

        Quaternion quat = Quaternion().FromAngles(m_phi, m_the, m_psi);
        return quat.RotateVector(vec);

        // if (m_psi != 0.0)
        // {
        //     imd = Yaw(imd, ref.m_hr);
        // }

        // if (m_the != 0.0)
        // {
        //     imd = Pitch(imd, ref.m_hl);
        // }

        // if (m_phi != 0.0)
        // {
        //     imd = Roll(imd, ref.m_ht);
        // }

        // return imd;
    }


    Vector OrientationTransformationHelper::ReverseTransformVector(const Vector &vec, const LVLHReference &ref) const
    {
        //Vector imd = vec;

        Quaternion quat = Quaternion().FromAngles(m_phi, m_the, m_psi).Inverse();
        return quat.RotateVector(vec);

        //return inv.TransformVector(vec, ref);
    }


}   /* namespace ns3 */